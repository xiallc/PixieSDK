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

#include <pixie_config.hpp>
#include <pixie_hw.hpp>
#include <pixie_param.hpp>
#include <pixie_util.hpp>

#include "json.hpp"

namespace xia
{
namespace pixie
{
namespace config
{

PIXIE_EXPORT void PIXIE_API
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
load(const std::string& filename,
     crate::crate& crate,
     module::number_slots& loaded)
{
    std::ifstream input_json_stream(filename);
    if (!input_json_stream) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json config: " + filename +
                    ": " + std::strerror(errno));
    }

    using json = nlohmann::json;

    json config = json::parse(input_json_stream);

    if (config.size() > crate.num_modules) {
        log(log::warning) << "too many module configs (" << config.size()
                          << "), crate only has " << crate.num_modules
                          << " modules ";
    }

    size_t mod = 0;
    auto ci = config.begin();

    while (mod < crate.num_modules) {
        auto& module = crate[mod];

        if (!module.online()) {
            log(log::warning) << "module " << mod << " not online, skipping";
        } else {
            auto& settings = *ci;

            auto rev =
                settings["metadata"]["hardware_revision"].get<std::string>();
            if (rev[0] != module.revision_label()) {
                log(log::warning) << "config module " << mod
                                  << " (rev " << rev
                                  << ") loading on to "
                                  << module.revision_label();
            }

            auto slot = settings["module"]["input"]["SlotID"];
            if (slot != module.slot) {
                log(log::warning) << "config module " << mod
                                  << " (slot " << slot
                                  << ") has moved to slot " << module.slot;
            }

            /*
             * Only check channel 0 until the metadata has arrays
             */
            auto& hw_config = module.configs[0];

            auto adc_bits = settings["metadata"]["adc_bit_resolution"];
            if (adc_bits != hw_config.adc_bits) {
                log(log::warning) << "config module " << mod
                                  << " (slot " << slot
                                  << ") ADC BIT resolution has changed from "
                                  << adc_bits << " to " << hw_config.adc_bits;
            }

            auto adc_msps = settings["metadata"]["adc_sampling_frequency"];
            if (adc_bits != hw_config.adc_bits) {
                log(log::warning) << "config module " << mod
                                  << " (slot " << slot
                                  << ") ADC sampling freq has changed from "
                                  << adc_msps << " to " << hw_config.adc_msps;
            }

            /*
             * Load module variables
             */
            for (auto& el : settings["module"]["input"].items()) {
                /*
                 * Load variables first and if not a parameter log a warning.
                 */
                if (param::is_module_var(el.key())) {
                    auto var = param::lookup_module_var(el.key());
                    auto& desc = module.module_var_descriptors[int(var)];
                    if (desc.writable()) {
                        if (desc.size != el.value().size()) {
                            log(log::warning) << module::module_label(module)
                                              << "size does not match config: "
                                              << el.key();
                        } else {
                            log(log::debug) << module::module_label(module)
                                            << "module var set: "
                                            << el.key();
                            if (desc.size > 1) {
                                for (size_t v = 0; v < desc.size; ++v) {
                                    module.write_var(var, el.value()[v], v, false);
                                }
                            } else {
                                module.write_var(var, el.value(), 0, false);
                            }
                        }
                    }
                } else if (!param::is_module_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod
                                      << " (slot " << slot
                                      << "): invalid variable: " << el.key();
                }
            }

            /*
             * Load channel variables
             */
            for (auto& el : settings["channel"]["input"].items()) {
                /*
                 * Load variables first and if not a parameter log a warning.
                 */
                if (param::is_channel_var(el.key())) {
                    auto var = param::lookup_channel_var(el.key());
                    auto& desc = module.channel_var_descriptors[int(var)];
                    if (desc.writable()) {
                        if ((el.value().size() % desc.size) != 0) {
                            log(log::warning) << module::module_label(module)
                                              << "size does not match config: "
                                              << el.key();
                        } else {
                            log(log::debug) << module::module_label(module)
                                            << "channel var set: "
                                            << el.key()
                                            << ": " << el.value();
                            for (size_t channel = 0;
                                 channel < module.num_channels &&
                                     channel * desc.size < el.value().size();
                                 ++channel) {
                                for (size_t v = 0; v < desc.size; ++v) {
                                    module.write_var(var,
                                                     el.value()[0],
                                                     channel, v, false);
                                }
                            }
                        }
                    }
                } else if (!param::is_channel_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod
                                      << " (slot " << slot
                                      << "): invalid variable: " << el.key();
                }
            }

            /*
             * Record the module had been loaded.
             */
            loaded.push_back(module::number_slot(module.number, module.slot));
        }

        ++ci;
        if (ci == config.end()) {
            break;
        }

        ++mod;
    }
}

void
unload(const std::string& json_file, crate::crate& crate)
{
    (void) json_file;
    (void) crate;
}

}
}
}
