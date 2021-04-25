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

            revision = mod_def.revision;
            eeprom_format = mod_def.eeprom_format;
            serial_num = mod_def.serial_num;
            num_channels = mod_def.num_channels;
            adc_bits = mod_def.adc_bits;
            adc_msps = mod_def.adc_msps;
            adc_clk_div = mod_def.adc_clk_div;
            fpga_clk_mhz = adc_msps / adc_clk_div;

            present_ = true;

            init_values();
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
    std::string md = mod_desc;

    if (delimiter != ' ') {
        std::transform(md.begin(), md.end(), md.begin(),
                       [delimiter](unsigned char c) -> unsigned char {
                           if (c == delimiter) return ' ';
                           return c; });
    }

    std::transform(md.begin(), md.end(), md.begin(),
                   [](unsigned char c) -> unsigned char {
                       if (std::isspace(c)) return ' ';
                       return c; });

    std::istringstream field_stream(md);

    module_def mod_def;

    field_stream >> mod_def.device_number
                 >> mod_def.slot
                 >> mod_def.revision
                 >> mod_def.eeprom_format
                 >> mod_def.serial_num
                 >> mod_def.num_channels
                 >> mod_def.adc_bits
                 >> mod_def.adc_msps
                 >> mod_def.adc_clk_div;

    mod_defs.push_back(mod_def);
}
}
}
}
