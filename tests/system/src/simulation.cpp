/*----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include <fstream>

#include <pixie_log.hpp>
#include <pixie_util.hpp>

#include "simulation.hpp"

namespace xia
{
namespace pixie
{
namespace sim
{
module_defs mod_defs;

module::~module()
{
}

void
module::open(size_t device_number)
{
    if (vmaddr != nullptr) {
        throw error(number, slot,
                    error::code::module_already_open,
                    "module has a vaddr");
    }

    for (auto& mod_def : mod_defs) {
        if (mod_def.num_channels != 0 &&
            device_number == mod_def.device_number) {
            log(log::info) << "sim: module: open: device=" << device_number;

            pci_memory = std::make_unique<uint8_t[]>(pci_addr_space_size);
            vmaddr = pci_memory.get();

            std::memset(vmaddr, 0, pci_addr_space_size);

            slot = mod_def.slot;
            revision = mod_def.revision;
            eeprom_format = mod_def.eeprom_format;
            serial_num = mod_def.serial_num;
            num_channels = mod_def.num_channels;
            adc_bits = mod_def.adc_bits;
            adc_msps = mod_def.adc_msps;
            adc_clk_div = mod_def.adc_clk_div;
            fpga_clk_mhz = adc_msps / adc_clk_div;

            var_defaults = mod_def.var_defaults;

            present_ = true;
            return;
        }
    }

    throw error(number, slot,
                error::code::module_initialize_failure,
                "no device found");
}

void
module::close()
{
    log(log::info) << "sim: module: close";
    present_ = false;
    vmaddr = nullptr;
    pci_memory.release();
}

void
module::probe()
{
    log(log::info) << "sim: module: probe";
    online_ = dsp_online = fippi_fpga = comms_fpga = true;
    init_values();
}

void
module::boot(bool boot_comms,
             bool boot_fippi,
             bool boot_dsp)
{
    log(log::info) << "sim: module: boot";
    if (boot_comms) {
        comms_fpga = true;
    }
    if (boot_fippi) {
        fippi_fpga = true;
    }
    if (boot_dsp) {
        dsp_online = true;
    }
}

void
module::initialize()
{
}

void
module::init_values()
{
    pixie::module::module::init_values();
    if (!var_defaults.empty()) {
        load_var_defaults(var_defaults);
    }
}

void
module::load_var_defaults(const std::string& file)
{
    log(log::info) << "sim: module: load var defaults: " << file;

    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw error(number, slot,
                    error::code::file_read_failure,
                    std::string("module var defaults open: ") + file +
                    ": " + std::strerror(errno));
    }

    for (std::string line; std::getline(input, line); ) {
        line = line.substr(0, line.find('#', 0));
        if (!line.empty()) {
            util::trim(line);
            util::strings label_value;
            util::split(label_value, line, '=');
            if (label_value.size() == 2) {
                label_value[1] =
                    label_value[1].substr(0, label_value[1].find('(', 0));
                if (param::is_module_var(label_value[0])) {
                    param::module_var var =
                        param::lookup_module_var(label_value[0]);
                    size_t index = static_cast<size_t>(var);
                    param::value_type value = std::stoul(label_value[1]);
                    module_vars[index].value[0] = value;
                    log(log::debug) << "sim: module: mod var: "
                                    << label_value[0] << '=' << label_value[1];
                } else if (param::is_channel_var(label_value[0])) {
                    param::channel_var var =
                        param::lookup_channel_var(label_value[0]);
                    size_t index = static_cast<size_t>(var);
                    param::value_type value = std::stoul(label_value[1]);
                    for (size_t channel = 0;
                         channel < num_channels;
                         ++channel) {
                        channels[channel].vars[index].value[0] = value;
                    }
                    log(log::debug) << "sim: module: chan var: "
                                    << label_value[0] << '=' << label_value[1];
                }
            }
        }
    }

    input.close();
}

void
crate::add_module()
{
    log(log::info) << "sim: module: add";
    modules.push_back(std::make_unique<module>());
}

module_def::module_def()
    : device_number(0),
      slot(0),
      revision(0),
      eeprom_format (0),
      serial_num(0),
      num_channels(0),
      adc_bits(0),
      adc_msps(0),
      adc_clk_div(0)
{
}

void
load_module_defs(const std::string mod_def_file)
{
    log(log::info) << "sim: load module defs: " << mod_def_file;
    std::ifstream input(mod_def_file, std::ios::in | std::ios::binary);
    if (!input) {
        throw error(error::code::file_read_failure,
                    std::string("module def file open: ") + mod_def_file +
                    ": " + std::strerror(errno));
    }
    load_module_defs(input);
    input.close();
    log(log::info) << "sim: module defs: " << mod_defs.size();
}

void
load_module_defs(std::istream& input)
{
    for (std::string line; std::getline(input, line); ) {
        if (!line.empty()) {
            add_module_def(line, ',');
        }
    }
}

void
add_module_def(const std::string mod_desc, const char delimiter)
{
    util::strings fields;
    util::split(fields, mod_desc, delimiter);

    module_def mod_def;

    for (auto field : fields) {
        util::strings label_value;
        util::split(label_value, field, '=');
        if (label_value.size() != 2) {
            throw error(error::code::invalid_value,
                        "invalid module definition: " + field);
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
            } else if (label_value[0] == "var-defaults") {
                mod_def.var_defaults = label_value[1];
            } else {
                throw error(error::code::invalid_value,
                            "invalid module definition: " + field);
            }
        } catch (error& e) {
            throw;
        } catch (...) {
            throw error(error::code::invalid_value,
                        "invalid module definition: bad value: " +
                        label_value[1]);
        }
    }

    log(log::info) << "sim: module desc: add: " << mod_desc;

    mod_defs.push_back(mod_def);
}
}
}
}
