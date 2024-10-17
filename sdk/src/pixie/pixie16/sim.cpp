/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file sim.cpp
 * @brief Implements a Pixie-16 simulation framework to facilitate testing
 */

#include <algorithm>
#include <cstring>
#include <fstream>

#include <pixie/log.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/utils/time.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/sim.hpp>

namespace xia {
namespace pixie {
namespace sim {
module_defs mod_defs;
eeprom_defs eep_defs;

static const xia::pixie::sim::firmware_set_defs default_firmware_defs = {
    {"version=n/a, revision=15, adc-msps=250, adc-bits=16, device=sys, mask=1, file=sys.bin",
     "version=n/a, revision=15, adc-msps=250, adc-bits=16, device=fippi, mask=0xf, file=fippi.bin",
     "version=n/a, revision=15, adc-msps=250, adc-bits=16, device=dsp, mask=1, file=dsp.ldr",
     "version=n/a, revision=15, adc-msps=250, adc-bits=16, device=var, mask=1, file=dsp.var"},
    {"version=n/a, revision=15, adc-msps=500, adc-bits=14, device=sys, mask=1, file=sys.bin",
     "version=n/a, revision=15, adc-msps=500, adc-bits=14, device=fippi, mask=0xf, file=fippi.bin",
     "version=n/a, revision=15, adc-msps=500, adc-bits=14, device=dsp, mask=1, file=dsp.ldr",
     "version=n/a, revision=15, adc-msps=500, adc-bits=14, device=var, mask=1, file=dsp.var"},
    {"version=n/a, revision=15, adc-msps=250, adc-bits=14, device=sys, mask=1, file=sys.bin",
     "version=n/a, revision=15, adc-msps=250, adc-bits=14, device=fippi, mask=0xf, file=fippi.bin",
     "version=n/a, revision=15, adc-msps=250, adc-bits=14, device=dsp, mask=1, file=dsp.ldr",
     "version=n/a, revision=15, adc-msps=250, adc-bits=14, device=var, mask=1, file=dsp.var"},
    {"version=n/a, revision=17, adc-msps=250, adc-bits=14, device=sys, mask=1, file=sys.bin",
     "version=n/a, revision=17, adc-msps=250, adc-bits=14, device=fippi, mask=0xf, file=fippi.bin",
     "version=n/a, revision=17, adc-msps=250, adc-bits=14, device=dsp, mask=1, file=dsp.ldr",
     "version=n/a, revision=17, adc-msps=250, adc-bits=14, device=var, mask=1, file=dsp.var"},
    {"version=n/a, revision=13, adc-msps=100, adc-bits=12, device=sys, mask=1, file=sys.bin",
     "version=n/a, revision=13, adc-msps=100, adc-bits=12, device=fippi, mask=0xf, file=fippi.bin",
     "version=n/a, revision=13, adc-msps=100, adc-bits=12, device=dsp, mask=1, file=dsp.ldr",
     "version=n/a, revision=13, adc-msps=100, adc-bits=12, device=var, mask=1, file=dsp.var"},
};

struct assembly : public xia::pixie::fixture::assembly {
    assembly(xia::pixie::module::module& module_);
    virtual ~assembly() override;
    virtual void open() override;
    virtual void close() override;
    virtual void initialize() override;
    virtual void online() override;
    virtual void forced_offline() override;
    virtual void fpga_comms_loaded() override;
    virtual void fpga_fippi_loaded() override;
    virtual void dsp_loaded() override;
    virtual void boot() override;
    virtual void erase_values() override;
    virtual void init_values() override;
    virtual void erase_channels() override;
    virtual void init_channels() override;
    virtual void sync_hw() override;
    virtual void sync_vars(const param::sync_mode sync_mode) override;
    virtual void set_dacs() override;
    virtual void get_traces() override;
    virtual void adjust_offsets() override;
    virtual void tau_finder() override;
};

assembly::assembly(xia::pixie::module::module& module__)
    : xia::pixie::fixture::assembly(module__, "sim") {
}
assembly::~assembly() {}
void assembly::open() {}
void assembly::close() {}
void assembly::initialize() {}
void assembly::online() {}
void assembly::forced_offline() {}
void assembly::fpga_comms_loaded() {}
void assembly::fpga_fippi_loaded() {}
void assembly::dsp_loaded() {}
void assembly::boot() {}
void assembly::erase_values() {}
void assembly::init_values() {}
void assembly::erase_channels() {}
void assembly::init_channels() { xia::pixie::fixture::assembly::init_channels(); }
void assembly::sync_hw() {}
void assembly::sync_vars(const param::sync_mode ) {}
void assembly::set_dacs() {}
void assembly::get_traces() {}
void assembly::adjust_offsets() {}
void assembly::tau_finder() {}

static void load_module_eeprom(const eeprom_data& data, xia::pixie::module::module& module_);

module::module(xia::pixie::backplane::backplane& backplane_)
    : xia::pixie::module::module(backplane_), fw_release(firmware::not_released),
      fw_type(firmware::firmware_set::set_type::undefined), init_online(true) {
}

module::~module() {}

void module::open(size_t device_number) {
    if (vmaddr != nullptr) {
        throw error(number, slot, error::code::module_already_open, "module has a vaddr");
    }

    for (auto& mod_def : mod_defs) {
        if (mod_def.num_channels != 0 && device_number == mod_def.device_number) {
            xia_log(log::info) << "sim: module: open: device=" << device_number;

            pci_memory = std::make_unique<uint8_t[]>(pci_addr_space_size);
            vmaddr = pci_memory.get();

            std::memset(vmaddr, 0, pci_addr_space_size);

            set_bus_device_number(device_number);
            slot = mod_def.slot;
            number = int(slot);
            revision = mod_def.revision;
            eeprom_format = mod_def.eeprom_format;
            serial_num = mod_def.serial_num;
            num_channels = mod_def.num_channels;
            max_channels = mod_def.num_channels;
            max_histogram_length = hw::large_histogram_length;
            auto eep_finder = [slot = mod_def.slot](eeprom_def def) {
                return slot == def.slot;
            };
            auto eep_data = std::find_if(eep_defs.begin(), eep_defs.end(), eep_finder);
            if (eep_data == eep_defs.end()) {
                if (revision == hw::rev_H) {
                    auto pid = mod_def.db_pid;
                    for (int p = 1; p <= mod_def.db_count; p++) {
                        hw::db_assembly db(pid, p);
                        db.label = eeprom.db_find_label(pid);
                        db.index = eeprom.dbs.size();
                        eeprom.dbs.push_back(db);

                        auto db_config = eeprom.db_config(pid);
                        db_config.fixture = hw::get_module_fixture(db.label);
                        size_t channels = eeprom.db_channel_count(db.index);
                        for (size_t c = 0; c < channels; ++c) {
                            eeprom.configs.push_back(db_config);
                        }
                    }
                    max_histogram_length = eeprom.configs[0].max_histogram_length;
                    for (auto& config : eeprom.configs) {
                        if (max_histogram_length != config.max_histogram_length) {
                            max_histogram_length = hw::large_histogram_length;
                            break;
                        }
                    }
                } else {
                    hw::config config;
                    config.adc_bits = mod_def.adc_bits;
                    config.adc_msps = mod_def.adc_msps;
                    config.adc_clk_div = mod_def.adc_clk_div;
                    config.fpga_clk_mhz = mod_def.adc_msps / mod_def.adc_clk_div;
                    eeprom.configs.resize(num_channels, config);
                }
                eeprom.num_channels = int(eeprom.configs.size());
                int index = 0;
                for (auto& cfg : eeprom.configs) {
                    cfg.index = index;
                    ++index;
                }
            } else {
                load_module_eeprom(eep_data->data, *this);
                eeprom.process();
            }

            run_config = hw::run::make(*this);
            fixtures = fixture::make(*this);
            fixtures->init_assemblies();

            start_fifo_services();

            fixtures->open();

            var_defaults = mod_def.var_defaults;

            opened_ = true;
            hw_word_read = [&self = *this](int reg) {
                return hw::read_word(self.vmaddr, reg);
            };
            hw_word_write = [&self = *this](int reg, hw::word val) {
                self.sim_reg(reg, val);
            };

            mib_base = "module." + std::to_string(slot) + '.';

            /*
             * Read-only, never disabled
             */
            mib::add_ro_int(mib_base + "slot", slot);
            mib::add_ro_int(mib_base + "num-channels", num_channels);
            mib::add_ro_int(mib_base + "max-channels", max_channels);
            mib::add_ro_int(mib_base + "serial-num", serial_num);
            mib::add_ro_int(mib_base + "revision", revision);

            mib::add_ro_int(mib_base + "eeprom.revision.major", eeprom.major_revision);
            mib::add_ro_int(mib_base + "eeprom.revision.minor", eeprom.minor_revision);
            mib::add_ro_int(mib_base + "eeprom.format", eeprom_format);

            mib::add_ro_int(mib_base + "sim.device-num", device_number);

            mib::add(mib_base + "sim.test.str", mib::type::string);
            mib::add(mib_base + "sim.test.int", mib::type::integer);
            mib::add(mib_base + "sim.test.dbl", mib::type::real);

            /*
             * Active, disabled when offline
             */
            mibs_size_t_rw.emplace_back(mib_base + "run.in", run_stats.in);
            mibs_size_t_rw.emplace_back(mib_base + "run.out", run_stats.out);
            mibs_size_t_rw.emplace_back(mib_base + "run.dma_in", run_stats.dma_in);
            mibs_size_t_rw.emplace_back(mib_base + "run.overflows", run_stats.overflows);
            mibs_size_t_rw.emplace_back(mib_base + "run.dropped", run_stats.dropped);
            mibs_size_t_rw.emplace_back(mib_base + "run.hw-overflows", run_stats.hw_overflows);
            mibs_double_rw.emplace_back(mib_base + "run.bandwidth", run_stats.bandwidth);
            mibs_double_rw.emplace_back(mib_base + "run.max-bandwidth", run_stats.max_bandwidth);
            mibs_double_rw.emplace_back(mib_base + "run.min-bandwidth", run_stats.min_bandwidth);

            return;
        }
    }

    throw error(number, slot, error::code::module_initialize_failure, "no device found");
}

void module::sim_reg(int reg, hw::word val) {
    switch(reg) {
        case hw::device::CSR:
            sim_csr(val);
            break;
        default:
            hw::write_word(vmaddr, reg, val);
    }
}

void module::sim_csr(hw::word val) {
    auto run_enabled = (val & (1 << hw::bit::RUNENA)) == 0;
    switch(control_task) {
        case hw::run::control_task::set_dacs:
        case hw::run::control_task::enable_input:
        case hw::run::control_task::ramp_offsetdacs:
        case hw::run::control_task::get_traces:
        case hw::run::control_task::program_fippi:
        case hw::run::control_task::get_baselines:
        case hw::run::control_task::adjust_offsets:
        case hw::run::control_task::tau_finder:
        case hw::run::control_task::reset_adc:
            if (run_enabled) {
                hw::write_word(vmaddr, hw::device::CSR, val);
            }
            break;
        case hw::run::control_task::fill_ext_fifo:
        case hw::run::control_task::nop:
        default:
            hw::write_word(vmaddr, hw::device::CSR, val);
    }
}

void module::close() {
    if (opened()) {
        xia_log(log::info) << "sim: module: close";
        if (online()) {
            if (run_active()) {
                run_end();
            }
        }

        if (fixtures) {
            fixtures->close();
            fixtures.reset();
        }

        eeprom.clear();

        force_offline();

        if (vmaddr != nullptr) {
            pci_memory.release();
            vmaddr = nullptr;
        }

        comms_fpga = false;
        fippi_fpga = false;
        dsp_online = false;

        number = -1;

        online_ = false;
        forced_offline_ = false;
        opened_ = false;

        mibs_size_t_rw.clear();
        mibs_double_rw.clear();

        mib::remove(mib_base + "run.min-bandwidth");
        mib::remove(mib_base + "run.max-bandwidth");
        mib::remove(mib_base + "run.bandwidth");
        mib::remove(mib_base + "run.hw-overflows");
        mib::remove(mib_base + "run.dropped");
        mib::remove(mib_base + "run.overflows");
        mib::remove(mib_base + "run.dma_in");
        mib::remove(mib_base + "run.out");
        mib::remove(mib_base + "run.in");

        mib::remove(mib_base + "sim.test.dbl");
        mib::remove(mib_base + "sim.test.int");
        mib::remove(mib_base + "sim.test.str");

        mib::remove(mib_base + "sim.device-num");

        mib::remove(mib_base + "eeprom.format");
        mib::remove(mib_base + "eeprom.revision.minor");
        mib::remove(mib_base + "eeprom.revision.major");

        mib::remove(mib_base + "revision");
        mib::remove(mib_base + "serial-num");
        mib::remove(mib_base + "max-channels");
        mib::remove(mib_base + "num-channels");
        mib::remove(mib_base + "slot");
    }
}

void module::probe(const firmware::firmware_set& firmware) {
    xia_log(log::info) << "sim: module: probe";
    online_ = dsp_online = fippi_fpga = comms_fpga = false;
    erase_values();
    erase_channels();
    init_values();
    init_channels();
    online_ = dsp_online = fippi_fpga = comms_fpga = init_online;
    if (online_) {
        fixtures->online();
    }
    fw_release = firmware.release;
    fw_type = firmware.type();
}

void module::boot(const boot_params& params, const firmware::firmware_set& firmware) {
    xia_log(log::info) << "sim: module: boot: slot=" << slot << " release=" << fw_release.to_string()
                       << " firmware.release=" << firmware.release.to_string();
    if (fw_release != firmware::not_released) {
        if (!firmware_resident(firmware.release) &&
            (!params.boot_comms || !params.boot_fippi || !params.boot_dsp)) {
            throw error(number, slot, error::code::module_initialize_failure,
                        "partial boot: firmware does not match resident firmware");
        }
    }
    online_ = false;
    mib_disable();
    auto boot_comms = params.boot_comms;
    auto boot_fippi = params.boot_fippi;
    auto boot_dsp = params.boot_dsp;
    init_values();
    if (boot_comms) {
        comms_fpga = true;
        fixtures->fpga_comms_loaded();
    }
    if (boot_fippi) {
        fippi_fpga = true;
        fixtures->fpga_fippi_loaded();
    }
    if (boot_dsp) {
        dsp_online = true;
        fixtures->dsp_loaded();
    }
    init_channels();
    online_ = comms_fpga && fippi_fpga && dsp_online;
    fw_release = firmware.release;
    fw_type = firmware.type();
    fixtures->boot();
    fixtures->online();
    mib_enable();
    write_var(param::module_var::SlotID, param::value_type(slot));
    write_word(hw::device::CSR, 1 << hw::bit::EXTFIFO_WML);
}

void module::initialize() {}

void module::firmware_release(
    firmware::release_type& release_, firmware::firmware_set::set_type& type) {
    release_ = fw_release;
    type = fw_type;
}

void module::init_values() {
    pixie::module::module::init_values();
    if (!var_defaults.empty()) {
        load_var_defaults(var_defaults);
    }
}

void module::dma_read(const hw::address source, hw::word_ptr values, const size_t size) {
    xia_log(log::debug) << module_label(*this) << "dma read: addr=0x" << std::hex << source
                        << " length=" << std::dec << size;

    online_check();
    size_t s = 0;
    while (s++ < size) {
        *values = read_word(int(source + s));
        ++values;
    }
}

void module::load_var_defaults(std::istream& input) {
    for (std::string line; std::getline(input, line);) {
        line = line.substr(0, line.find('#', 0));
        if (!line.empty()) {
            util::string::trim(line);
            util::string::strings label_value;
            util::string::split(label_value, line, '=');
            if (label_value.size() == 2) {
                label_value[1] = label_value[1].substr(0, label_value[1].find('(', 0));
                if (param::is_module_var(label_value[0])) {
                    param::module_var var = param::lookup_module_var(label_value[0]);
                    size_t index = static_cast<size_t>(var);
                    param::value_type value = std::stoul(label_value[1]);
                    module_vars[index].value[0].value = value;
                    module_vars[index].value[0].dirty = true;
                    xia_log(log::debug)
                        << "sim: module: mod var: " << label_value[0] << '=' << label_value[1];
                } else if (param::is_channel_var(label_value[0])) {
                    param::channel_var var = param::lookup_channel_var(label_value[0]);
                    size_t index = static_cast<size_t>(var);
                    param::value_type value = std::stoul(label_value[1]);
                    for (size_t channel = 0; channel < num_channels; ++channel) {
                        channels[channel].vars[index].value[0].value = value;
                        channels[channel].vars[index].value[0].dirty = true;
                    }
                    xia_log(log::debug)
                        << "sim: module: chan var: " << label_value[0] << '=' << label_value[1];
                }
            }
        }
    }
}

void module::load_var_defaults(const std::string& file) {
    xia_log(log::info) << "sim: module: load var defaults: " << file;

    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw error(
            number, slot, error::code::file_read_failure,
            std::string("module var defaults open: ") + file + ": " + std::strerror(errno));
    }

    load_var_defaults(input);

    input.close();
}

crate::crate(bool init_online_) : init_online(init_online_) {}

crate::~crate() {
    for (auto slot : slots) {
        slot->close();
    }
}

void crate::create_module_slots() {
    xia_log(log::info) << "sim: module: create crate";
    num_slots = hw::max_slots;
    for (size_t s = 0; s < num_slots; ++s) {
        auto mod = std::make_shared<module>(backplane);
        mod->init_online = init_online;
        slots.push_back(mod);
    }
}

module_def::module_def()
    : device_number(0), slot(0), revision(0), eeprom_format(0), serial_num(0), num_channels(0),
      adc_bits(0), adc_msps(0), adc_clk_div(0) {}

void load_firmware_sets(firmware::system& firmwares) {
    load_firmware_sets(firmwares, default_firmware_defs);
}

void load_firmware_sets(firmware::system& firmwares, const firmware_set_defs& set_defs) {
    xia::pixie::firmware::release_type release = xia::pixie::firmware::not_released;
    for (auto& fw_set_def : set_defs) {
        xia::pixie::firmware::firmware_set fw_set(release, "");
        for (auto& fw_def : fw_set_def) {
            auto fw = xia::pixie::firmware::parse(release, fw_def, ',');
            if (release == xia::pixie::firmware::not_released && fw.release != release) {
                release = fw.release;
            }
            fw_set.add(fw);
        }
        fw_set.release = release;
        xia::pixie::firmware::add(firmwares, fw_set);
    }
}

void load_module_defs(const std::string mod_def_file) {
    xia_log(log::info) << "sim: load module defs: " << mod_def_file;
    std::ifstream input(mod_def_file, std::ios::in | std::ios::binary);
    if (!input) {
        throw error(
            error::code::file_read_failure, std::string("module def file open: ") +
            mod_def_file + ": " + std::strerror(errno));
    }
    load_module_defs(input);
    input.close();
    xia_log(log::info) << "sim: module defs: " << mod_defs.size();
}

void load_module_defs(std::istream& input) {
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            add_module_def(line, ',');
        }
    }
}

void add_module_def(const std::string mod_desc, const char delimiter) {
    util::string::strings fields;
    util::string::split(fields, mod_desc, delimiter);

    module_def mod_def;

    for (auto field : fields) {
        util::string::strings label_value;
        util::string::split(label_value, field, '=');
        if (label_value.size() != 2) {
            throw error(error::code::invalid_value, "invalid module definition: " + field);
        }

        try {
            if (label_value[0] == "device-number") {
                mod_def.device_number = std::stoul(label_value[1]);
            } else if (label_value[0] == "slot") {
                mod_def.slot = std::stoul(label_value[1]);
            } else if (label_value[0] == "revision") {
                mod_def.revision = std::stoul(label_value[1]);
            } else if (label_value[0] == "eeprom-format") {
                mod_def.eeprom_format = std::stoul(label_value[1]);
            } else if (label_value[0] == "serial-num") {
                mod_def.serial_num = std::stoul(label_value[1]);
            } else if (label_value[0] == "num-channels") {
                mod_def.num_channels = std::stoul(label_value[1]);
            } else if (label_value[0] == "adc-bits") {
                mod_def.adc_bits = std::stoul(label_value[1]);
            } else if (label_value[0] == "adc-msps") {
                mod_def.adc_msps = std::stoul(label_value[1]);
            } else if (label_value[0] == "adc-clk-div") {
                mod_def.adc_clk_div = std::stoul(label_value[1]);
            } else if (label_value[0] == "db_pid") {
                mod_def.db_pid = std::stoul(label_value[1]);
            } else if (label_value[0] == "db_count") {
                mod_def.db_count = std::stoul(label_value[1]);
            } else if (label_value[0] == "var-defaults") {
                mod_def.var_defaults = label_value[1];
            } else {
                throw error(error::code::invalid_value, "invalid module definition: " + field);
            }
        } catch (error&) {
            throw;
        } catch (...) {
            throw error(error::code::invalid_value,
                        "invalid module definition: bad value: " + label_value[1]);
        }
    }

    xia_log(log::info) << "sim: module desc: add: " << mod_desc;

    mod_defs.push_back(mod_def);
}

void clear_module_defs() {
    mod_defs.clear();
}

void load_eeprom_defs(const eeprom_slot_def& defs) {
    auto& slots = std::get<0>(defs);
    auto& datas = std::get<1>(defs);
    if (slots.size() != datas.size()) {
        throw error(error::code::invalid_value, "Number of slots and EEPROMs don't match");
    }
    for (size_t i = 0; i < slots.size(); i++) {
        add_eeprom_def(slots[i], datas[i]);
    }
}

void add_eeprom_def(const int slot, const eeprom_data& data) {
    eeprom_def eep_def;
    eep_def.data = data;
    eep_def.slot = slot;
    eep_defs.push_back(eep_def);
}

void clear_eeprom_defs() {
    eep_defs.clear();
}

template<typename T>
static T get_value(const std::string& opt, int base = 9) {
    T value = 0;
    try {
        value = T(std::stoul(opt, nullptr, base));
    } catch (std::invalid_argument& ) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range& ) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

static void load_module_eeprom(const eeprom_data& data, xia::pixie::module::module& module_) {
    auto& eeprom = module_.eeprom;
    eeprom.clear();
    eeprom.data.resize(xia::pixie::hw::eeprom_block_size);
    for (auto& s : data) {
        xia::util::string::strings ss;
        xia::util::string::split(ss, s);
        if (ss.size() != 17) {
            throw std::runtime_error("invalid EEPROM format");
        }
        eeprom_addr a = get_value<eeprom_addr>(ss[0], 16);
        if ((a + 16) > eeprom.data.size()) {
            throw std::runtime_error("invalid EEPROM address: " + ss[0]);
        }
        for (int b = 0; b < 7; ++b) {
            eeprom.data[a + b] = get_value<uint8_t>(ss[b + 1], 16);
        }
        xia::util::string::strings ss7_8;
        xia::util::string::split(ss7_8, ss[8], '-');
        if (ss7_8.size() != 2) {
            throw std::runtime_error("invalid EEPROM format");
        }
        eeprom.data[a + 7] = get_value<uint8_t>(ss7_8[0], 16);
        eeprom.data[a + 8] = get_value<uint8_t>(ss7_8[1], 16);
        for (int b = 9; b < 16; ++b) {
            eeprom.data[a + b] = get_value<uint8_t>(ss[b], 16);
        }
    }
    return;
};
}  // namespace sim
}  // namespace pixie
}  // namespace xia
