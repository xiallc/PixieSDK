/*----------------------------------------------------------------------
* Copyright (c) 2021 - 2021, XIA LLC
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

#ifndef PIXIE_SDK_SYSTEM_SIMULATION_HPP
#define PIXIE_SDK_SYSTEM_SIMULATION_HPP

#include <iostream>

#include <pixie_crate.hpp>
#include <pixie_error.hpp>
#include <pixie_module.hpp>

namespace xia
{
namespace pixie
{
namespace sim
{
typedef xia::pixie::error::error error;

/*
 * Simulated a module.
 */
class module
    : public xia::pixie::module::module
{
public:
    static const size_t pci_addr_space_size = 8 * 1024 * 1024;

    typedef xia::pixie::module::error error;

    ~module() override;

    void open(size_t device_number) override;
    void close() override;
    void probe() override;
    void boot(bool boot_comms = true,
              bool boot_fippi = true,
              bool boot_dsp = true) override;
    void initialize() override;
    void init_values() override;

    void load_var_defaults(const std::string& file);
    void load_var_defaults(std::istream& input);

    std::unique_ptr<uint8_t[]> pci_memory;
    std::string var_defaults;
};

/*
 * Simulated a crate. Crates the simulated module.
 */
class crate
    : public xia::pixie::crate::crate
{
public:
    typedef xia::pixie::crate::error error;

    void add_module() override;
};

/*
 * Module definition.
 */
struct module_def
{
    size_t device_number;
    int slot;
    int revision;
    int eeprom_format;
    int serial_num;
    int num_channels;
    int adc_bits;
    int adc_msps;
    int adc_clk_div;
    std::string var_defaults;

    module_def();
};

typedef std::vector<module_def> module_defs;

extern module_defs mod_defs;

void load_module_defs(const std::string mod_def_file);
void load_module_defs(std::istream& input);
void add_module_def(const std::string mod_desc, const char delimiter = ' ');
}
}
}

#endif // PIXIE_SDK_SYSTEM_SIMULATION_HPP
