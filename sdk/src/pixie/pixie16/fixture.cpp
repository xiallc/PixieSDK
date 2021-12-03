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

/** @file fixture.cpp
 * @brief Implements per channel hardware specific support for the Pixie-16 modules.
 */

#include <climits>
#include <cstring>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace fixture {
/**
 * UserIn save to hold and restore UserIn variables.
 */
struct userin_save {
    hw::memory::dsp dsp;
    hw::address address;

    hw::word userin_0;
    hw::word userin_1;

    userin_save(pixie::module::module& module);

    void update(const hw::word& db_index, const hw::word& db_channel);

    ~userin_save();
};

/**
 * ADC trace analyize
 *
 * This is a simple approach to detecting the average voltage.
 */
struct channel_baseline {
    int channel;
    int samples;
    int baseline;
    int minimum;
    int maximum;

    channel_baseline();

    void start();
    void end();

    void update(hw::adc_word sample);

    bool operator==(const channel_baseline& other) const;
    bool operator!=(const channel_baseline& other) const;
};

using channel_baselines = std::vector<channel_baseline>;

/**
 * The daughter board fixture
 */
struct db : public channel {
    /**
     * ADC swapped state.
     *
     * If the DSP is not loaded the swap state is unknown.
     */
    enum adc_swap_state {
        adc_boot_state,
        adc_unswapped,
        adc_swapped
    };
    /**
     * The number is the position. The DBs are sorted by position so
     * asking for the index by channel returns the DB number.
     */
    int number;

    /**
     * Base channel for the daughter board
     */
    int base;

    /**
     * Channel offset relative to the fixture.
     */
    int offset;

    /**
     * Dual ADC swapped state. This is true if the data is being clock clocked
     * on the wrong edge.
     */
    adc_swap_state adc_state;

    db(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void acquire_adc() override;
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    virtual void set(const std::string item, bool value);
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, int& value);

    virtual void report(std::ostream& out) const override;
};

/**
 * The DB04 fixture
 */
struct db04 : public db {
    db04(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void set_dac(param::value_type value) override;
};

/**
 * Module has AFE DB fixtures
 */
struct afe_dbs : public module {
    enum limits {
        max_dbs = 4
    };

    std::array<hw::word, max_dbs> adcctrl;

    afe_dbs(pixie::module::module& module_);

    virtual void fgpa_fippi_loaded() override;
    virtual void boot() override;
};

static void unsupported_op(const std::string what) {
    throw error::error(error::code::internal_failure, "invalid fixture op: " + what);
}

static void set_channel_voffset(pixie::module::module& mod, double voffset, int step) {
    for (int chan = 0; chan < static_cast<int>(mod.num_channels); chan += step) {
        mod.channels[chan].voffset(voffset);
    }
    mod.set_dacs();
}

static void analyze_channel_baselines(pixie::module::module& mod, channel_baselines& baselines) {
    mod.get_traces();
    for (int channel = 0; channel < static_cast<int>(mod.num_channels); ++channel) {
        xia::pixie::hw::adc_trace trace;
        mod.read_adc(channel, trace, false);
        channel_baseline baseline;
        for (auto sample : trace) {
            baseline.update(sample);
        }
        baseline.end();
        baselines.push_back(baseline);
    }
}

userin_save::userin_save(pixie::module::module& module) : dsp(module) {
    address = module.module_var_descriptors[int(param::module_var::UserIn)].address;
    userin_0 = dsp.read(0, address);
    userin_1 = dsp.read(1, address);
}

void userin_save::update(const hw::word& db_index, const hw::word& db_channel) {
    dsp.write(0, address, db_index);
    dsp.write(1, address, db_channel);
}

userin_save::~userin_save() {
    dsp.write(0, address, userin_0);
    dsp.write(1, address, userin_1);
}

channel_baseline::channel_baseline() {
    start();
}

void channel_baseline::update(hw::adc_word sample) {
    ++samples;
    baseline += sample;
    if (sample < minimum) {
        minimum = sample;
    }
    if (sample > maximum) {
        maximum = sample;
    }
}

void channel_baseline::start() {
    samples = 0;
    baseline = 0;
    minimum = INT_MAX;
    maximum = INT_MIN;
}

void channel_baseline::end() {
    baseline /= samples;
}

bool channel_baseline::operator==(const channel_baseline& other) const
{
    return baseline >= (other.baseline - 100) && baseline <= (other.baseline + 100);
}

bool channel_baseline::operator!=(const channel_baseline& other) const
{
    return !(*this == other);
}

db::db(pixie::channel::channel& module_channel_, const hw::config& config_)
    : channel(module_channel_, config_) {
    pixie::module::module& mod = get_module();
    label = hw::get_module_fixture_label(config_.fixture);
    number = mod.eeprom.db_find(static_cast<int>(module_channel.number));
    base =  mod.eeprom.db_channel_base(number);
    offset = static_cast<int>(module_channel.number - base);
    adc_state = adc_boot_state;
}

void db::acquire_adc() {
    pixie::module::module& module = get_module();
    {
        userin_save userins(module);
        userins.update(number, offset);
        hw::run::control_run_on_dsp(module, hw::run::control_task::get_traces);
    }
    /*
     * Make sure the buffer is the maximum size a user can ask for. The
     * allocation is only done if the buffer is used and it cannot be released
     * because the user can ask to read it at any time.
     */
    const size_t size = module_channel.fixture->config.max_adc_trace_length;
    if (module_channel.adc_trace.size() != size) {
        module_channel.adc_trace.resize(size);
    }
    hw::memory::dsp dsp(module);
    hw::adc_trace_buffer adc_trace;
    dsp.read(hw::memory::IO_BUFFER_ADDR, adc_trace, size / 2);
    for (size_t w = 0; w < size / 2; ++w) {
        module_channel.adc_trace[w * 2] = hw::adc_word(adc_trace[w] & 0xffff);
        module_channel.adc_trace[w * 2 + 1] = hw::adc_word((adc_trace[w] >> 16) & 0xffff);
    }
}

void db::read_adc(hw::adc_word* buffer, size_t size) {
    const size_t copy_size =
        (size < module_channel.adc_trace.size() ?
         size : module_channel.adc_trace.size()) * sizeof(hw::adc_word);
    memcpy(buffer, module_channel.adc_trace.data(), copy_size);
}

void db::set(const std::string item, bool value) {
    if (item == "ADC_SWAP") {
        if (adc_state == adc_boot_state) {
            adc_state = value ? adc_swapped : adc_unswapped;
        }
    } else {
        unsupported_op("db: set: unsupported item");
    }
}

void db::get(const std::string item, bool& value) {
    if (item == "ADC_SWAP") {
        value = (adc_state == adc_swapped);
    } else {
        unsupported_op("db: get: unsupported item");
    }
}

void db::get(const std::string item, int& value) {
    if (item == "DB_NUMBER") {
        value = number;
    } else if (item == "DB_OFFSET") {
        value = offset;
    } else {
        unsupported_op("db: get: unsupported item");
    }
}

void db::report(std::ostream& out) const {
    channel::report(out);
    out << "DB Number      : " << number << std::endl
        << "DB Base        : " << base << std::endl
        << "DB Offset      : " << offset << std::endl
        << "ADC swap state : ";
    switch (adc_state) {
    case adc_boot_state:
        out << "boot state";
        break;
    case adc_unswapped:
        out << "not swapped";
        break;
    case adc_swapped:
        out << "swapped";
        break;
    }
    out << std::endl;
}

db04::db04(pixie::channel::channel& module_channel_, const hw::config& config_)
    : db(module_channel_, config_) {
}

void db04::set_dac(param::value_type value) {
    pixie::module::module& mod = get_module();
    if (value > 65535) {
        throw error::error(error::code::invalid_value,
                           pixie::module::module_label(mod, "DB04") + "invalid DAC offset: channel=" +
                           std::to_string(module_channel.number));
    }
    /*
     * Select the module port
     */
    mod.select_port(number + 1);
    /*
     * Address bit 1 selects DAC for the upper 4 channels. Clear bit 0 and set
     * bit 1 if the DB channel offset is less than 4.
     */
    hw::word dac_addr = 0x20 | ((offset < 4 ? 1 : 0) << 1);
    /*
     * Compensate for PCB ADC swapping:
     *
     * Channel offset DAC Output
     *     0, 4           B (1)
     *     1, 5           C (2)
     *     2, 6           A (0)
     *     3, 7           D (3)
     */
    hw::word dac_ctrl = 0x30;
    switch (offset) {
    case 0:
    case 4:
        dac_ctrl += 1;
        break;
    case 1:
    case 5:
        dac_ctrl += 2;
        break;
    case 2:
    case 6:
        dac_ctrl += 0;
        break;
    case 3:
    case 7:
        dac_ctrl += 3;
        break;
    default:
        break;
    }
    /*
     * CFG_DAC expacts [addr(8), ctrl(8), data(16)]
     */
    const hw::word dac = (dac_addr << 24) | (dac_ctrl << 16) | value;
    log(log::debug) << pixie::module::module_label(mod, "fixture: db04")
                    << "db=" << number
                    << " db_channel=" << offset
                    << std::hex
                    << " dac_addr=0x" << dac_addr
                    << " dac_ctrl=0x" << dac_ctrl
                    << " dac_value=0x" << value
                    << " write=0x" << dac;
    mod.write_word(hw::device::CFG_DAC, dac);
     /*
     * It takes about 4ms to clock out the 32 bits
     */
    hw::wait(6000);
}

afe_dbs::afe_dbs(pixie::module::module& module__)
    : module(module__), adcctrl { } {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "create";
}

void afe_dbs::fgpa_fippi_loaded() {
    adcctrl = { };
}

void afe_dbs::boot() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "boot";

    util::timepoint tp(true);

    /*
     * Set the voffset for all channels to the low rail
     */
    channel_baselines bl_same;
    set_channel_voffset(module_, -1.5, 1);
    analyze_channel_baselines(module_, bl_same);

    /*
     * Move the voffset for the even channels to high rail
     */
    channel_baselines bl_moved;
    set_channel_voffset(module_, 1.5, 2);
    analyze_channel_baselines(module_, bl_moved);

    /*
     * Check all the channels and swap of the ADCs if required.
     */
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        bool swapped = false;
        if ((chan % 2) == 0) {
            if (bl_same[chan] == bl_moved[chan]) {
                swapped = true;
            }
        } else {
            if (bl_same[chan] != bl_moved[chan]) {
                swapped = true;
            }
        }
        module_.channels[chan].fixture->set("ADC_SWAP", swapped);
        if (swapped) {
            int chan_db;
            int chan_offset;
            module_.channels[chan].fixture->get("DB_NUMBER", chan_db);
            module_.channels[chan].fixture->get("DB_OFFSET", chan_offset);
            if (chan_db >= max_dbs) {
                throw pixie::module::error(module_.number, module_.slot,
                                           pixie::module::error::code::module_initialize_failure,
                                           "invalid DB number for channel: " + std::to_string(chan));
            }
            hw::word last_adcctrl = adcctrl[chan_db];
            adcctrl[chan_db] |= 1 << (chan_offset / 2);
            log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                            << "boot: adc_swap: db="
                            << chan_db << " offset=" << chan_offset
                            << " adcctrl=0x" << std::hex << adcctrl[chan_db];
            if (adcctrl[chan_db] != last_adcctrl) {
                hw::memory::fippi fippi(module_);
                fippi.write(hw::fippi_addr(chan_db, hw::fippi::ADCCTRL), adcctrl[chan_db]);
            }
        }
    }

    /*
     * Verify.
     */
    bool failed = false;
    channel_baselines bl_verify;
    analyze_channel_baselines(module_, bl_verify);
    for (int chan = 0; chan < static_cast<int>(module_.num_channels); ++chan) {
        if ((chan % 2) == 0) {
            if (bl_same[chan] == bl_verify[chan]) {
                failed = true;
                log(log::error) << pixie::module::module_label(module_, "fixture: afe_dbs")
                                << "boot: ADC swap failed: " << chan;
            }
        } else {
            if (bl_same[chan] != bl_verify[chan]) {
                failed = true;
                log(log::error) << pixie::module::module_label(module_, "fixture: afe_dbs")
                                << "boot: ADC swap failed: " << chan;
            }
        }
    }

    set_channel_voffset(module_, 0, 1);

    if (failed) {
        throw pixie::module::error(module_.number, module_.slot,
                                   pixie::module::error::code::module_initialize_failure,
                                   "DB AE ADC swap failure");
    }

    log(log::debug) << pixie::module::module_label(module_, "fixture: afe_dbs")
                    << "boot: duration=" << tp;
}

channel::channel(pixie::channel::channel& module_channel_, const hw::config& config_)
    : module_channel(module_channel_), label("motherboard"),
      config(config_) {
}

channel::~channel() {
    close();
}

void channel::open() {
    /* Do nothing */
}

void channel::close() {
    /* Do nothing */
}

void channel::set_dac(param::value_type ) {
    /* Do nothing */
}

void channel::acquire_adc() {
    unsupported_op("ADC acquire is using the DSP");
}

void channel::read_adc(hw::adc_word* buffer, size_t size) {
    (void) buffer;
    (void) size;
    unsupported_op("read ADC is using the DSP");
}

void channel::report(std::ostream& out) const {
    out << "Fixture        : " << label << std::endl;
    config.report(out);
}

void channel::set(const std::string item, bool ) {
    (void) item;
    unsupported_op("not set support: bool");
}

void channel::set(const std::string item, int value) {
    (void) item;
    (void) value;
    unsupported_op("no set support: int");
}

void channel::set(const std::string item, hw::word value) {
    (void) item;
    (void) value;
    unsupported_op("no set support: hw::word");
}

void channel::get(const std::string item, bool& value) {
    (void) item;
    (void) value;
    unsupported_op("no get support: bool");
}

void channel::get(const std::string item, int& ) {
    (void) item;
    unsupported_op("no get support: int");
}

void channel::get(const std::string item, hw::word& ) {
    (void) item;
    unsupported_op("no get support: hw::word");
}

pixie::module::module& channel::get_module() {
    return module_channel.module;
}

module::module(pixie::module::module& module__)
    : module_(module__), label("none") {
    /* Do nothing */
}

module::~module() {
    /* Do nothing */
}

void module::open() {
    /* Do nothing */
}

void module::close() {
    /* Do nothing */
}

void module::initialize() {
    /* Do nothing */
}

void module::online() {
    /* Do nothing */
}

void module::forced_offline() {
    /* Do nothing */
}

void module::fgpa_comms_loaded() {
    /* Do nothing */
}

void module::fgpa_fippi_loaded() {
    /* Do nothing */
}

void module::dsp_loaded() {
    /* Do nothing */
}

void module::boot() {
    /* Do nothing */
}

void module::erase_values() {
    /* Do nothing */
}

void module::init_values() {
    /* Do nothing */
}

void module::erase_channels() {
    /* Do nothing */
}

void module::init_channels() {
    log(log::debug) << pixie::module::module_label(module_, "fixture: module")
                    << "init-channels: create channel fixtures";
    for (auto chan = 0; chan < module_.num_channels; ++chan) {
        module_.channels[chan].fixture =
            fixture::make(module_.channels[chan], module_.eeprom.configs[chan]);
    }
}

void module::sync_hw() {
    /* Do nothing */
}

void module::sync_vars() {
    /* Do nothing */
}

channel_ptr make(pixie::channel::channel& module_channel, const hw::config& config) {
    channel_ptr chan_fixture;
    switch (config.fixture) {
    case hw::module_fixture::DB04:
        chan_fixture = std::make_shared<db04>(module_channel, config);
        break;
    default:
        chan_fixture = std::make_shared<channel>(module_channel, config);
        break;
    }
    chan_fixture->open();
    return chan_fixture;
}

module_ptr make(pixie::module::module& module_) {
    module_ptr mod_fixtures;
    switch (module_.get_rev_tag()) {
    case hw::rev_tag::rev_H:
        mod_fixtures = std::make_shared<afe_dbs>(module_);
        break;
    default:
        mod_fixtures = std::make_shared<module>(module_);
        break;
    }
    return mod_fixtures;
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
