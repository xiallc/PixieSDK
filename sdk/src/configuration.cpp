/**----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
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
*----------------------------------------------------------------------**/
/// @file configuration.cpp
/// @brief Implementation for the configuration header
/// @author S. V. Paulauskas
/// @date April 13, 2021

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <configuration.hpp>
#include <pixie_hw.hpp>
#include <pixie_param.hpp>

#include "json.hpp"

namespace xia {
namespace pixie {
namespace config {

void
read(const std::string& config_file_name, configuration& cfg)
{
    std::ifstream input(config_file_name, std::ios::in);
    if (input.fail()) {
        throw error(error::code::file_open_failure,
                    "open: " + config_file_name + ": " + std::strerror(errno));
    }

    input >> cfg.num_modules;
    if (cfg.num_modules == 0 || cfg.num_modules > hw::max_slots) {
        throw error(error::code::config_invalid_param,
                    "invalid number of modules");
    }

    cfg.slot_map.clear();
    for (int num = 0; num < cfg.num_modules; num++) {
        int slot;
        if (input >> slot) {
            cfg.slot_map.push_back(module::number_slot(num, slot));
        } else {
            throw error(error::code::config_invalid_param,
                        "invalid slot");
        }
    }

    input >> cfg.com_fpga_config;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid COM FPGA file name");
    }

    input >> cfg.sp_fpga_config;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid FP FPGA file name");
    }

    std::string trig_holder;
    input >> trig_holder;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid Trigg file name");
    }

    input >> cfg.dsp_code;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid DSP code file name");
    }

    input >> cfg.dsp_param;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid DSP parameters file name");
    }

    input >> cfg.dsp_var;
    if (!input) {
        throw error(error::code::config_invalid_param,
                    "invalid DSP variables file name");
    }

    input.close();
}

void
read(const std::string& filename, module::module& module)
{
    std::ifstream input_json_stream(filename);
    if (!input_json_stream) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json file: " + filename + ": " + std::strerror(errno));
    }

    (void) module;

    nlohmann::json jf = nlohmann::json::parse(input_json_stream);

    for (auto& config : jf) {
        nlohmann::json metadata = config["metadata"];
        nlohmann::json module_parameters_json = config["module_parameters"];
        nlohmann::json channel_parameters_json = config["channel_parameters"];
    }
}

}
}
}
