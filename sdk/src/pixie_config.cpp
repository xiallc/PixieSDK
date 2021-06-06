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
using json = nlohmann::json;

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

static void
throw_json_error(json::exception& e, const std::string& what)
{
    throw error(error::code::config_json_error, what + ": " + e.what());
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

    json config;

    try {
        config = json::parse(input_json_stream);
    } catch (json::exception& e) {
        throw_json_error(e, "parse config");
    }

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

            if (!settings.contains("metadata")) {
                throw error(error::code::config_json_error,
                            "'metadata' not found");
            }

            if (!settings.contains("module")) {
                throw error(error::code::config_json_error,
                            "'module' not found");
            }

            if (!settings.contains("channel")) {
                throw error(error::code::config_json_error,
                            "'channel' not found");
            }

            auto metadata = settings["metadata"];
            auto moddata = settings["module"];
            auto chandata = settings["channel"];

            if (!moddata.contains("input")) {
                throw error(error::code::config_json_error,
                            "module 'input' not found");
            }

            if (!chandata.contains("input")) {
                throw error(error::code::config_json_error,
                            "channel 'input' not found");
            }

            try {
                auto rev = metadata["hardware_revision"].get<std::string>();
                if (rev[0] != module.revision_label()) {
                    log(log::warning) << "config module " << mod
                                      << " (rev " << rev
                                      << ") loading on to "
                                      << module.revision_label();
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config rev");
            }

            try {
                auto slot = moddata["input"]["SlotID"];
                if (slot != module.slot) {
                    log(log::warning) << "config module " << mod
                                      << " (slot " << slot
                                      << ") has moved to slot "
                                      << module.slot;
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config slot-id");
            }

            /*
             * Only check channel 0 until the metadata has arrays
             */
            auto& hw_config = module.configs[0];

            try {
                auto adc_bits = metadata["adc_bit_resolution"];
                if (adc_bits != hw_config.adc_bits) {
                    log(log::warning) << "config module " << mod
                                      << " (slot " << module.slot
                                      << ") ADC BIT count has changed from "
                                      << adc_bits << " to "
                                      << hw_config.adc_bits;
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config adc-bits");
            }

            try {
                auto adc_msps = metadata["adc_sampling_frequency"];
                if (adc_msps != hw_config.adc_msps) {
                    log(log::warning) << "config module " << mod
                                      << " (slot " << module.slot
                                      << ") ADC sampling freq changed from "
                                      << adc_msps << " to "
                                      << hw_config.adc_msps;
                }
            } catch (json::exception& e) {
                throw_json_error(e, "config adc-freq");
            }

            /*
             * Load module variables
             */
            for (auto& el : moddata["input"].items()) {
                /*
                 * Load variables first and if not a variable check if it is a
                 * parameter and if not a parameter log a warning. This puts
                 * variables before parameters and ignores parameters if
                 * present.
                 */
                if (param::is_module_var(el.key())) {
                    auto var = param::lookup_module_var(el.key());
                    auto& desc = module.module_var_descriptors[int(var)];
                    if (desc.writable()) {
                        if (desc.size != el.value().size()) {
                            log(log::warning) << module::module_label(module)
                                              << "size does not match: "
                                              << el.key();
                        } else {
                            log(log::debug) << module::module_label(module)
                                            << "module var set: "
                                            << el.key();
                            if (desc.size > 1) {
                                for (size_t v = 0; v < desc.size; ++v) {
                                    try {
                                        module.write_var(var,
                                                         el.value()[v],
                                                         v,
                                                         false);
                                    } catch (json::exception& e) {
                                        auto s = el.key() + ": " +
                                            std::string(el.value());
                                        throw_json_error(e, s);
                                    }
                                }
                            } else {
                                try {
                                    module.write_var(var,
                                                     el.value(),
                                                     0,
                                                     false);
                                } catch (json::exception& e) {
                                    auto s = el.key() + ": " +
                                        std::string(el.value());
                                    throw_json_error(e, s);
                                }
                            }
                        }
                    }
                } else if (!param::is_module_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod
                                      << " (slot " << module.slot
                                      << "): invalid variable: " << el.key();
                }
            }

            /*
             * Load channel variables
             */
            for (auto& el : chandata["input"].items()) {
                /*
                 * Load variables first and if not a variable check if it is a
                 * parameter and if not a parameter log a warning. This puts
                 * variables before parameters and ignores parameters if
                 * present.
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
                            size_t vchannels = el.value().size() / desc.size;
                            for (size_t channel = 0;
                                 channel < module.num_channels &&
                                     channel < vchannels &&
                                     channel * desc.size < el.value().size();
                                 ++channel) {
                                size_t vbase = channel * desc.size;
                                for (size_t v = 0; v < desc.size; ++v) {
                                    try {
                                        module.write_var(var,
                                                         el.value()[vbase + v],
                                                         channel, v, false);
                                    } catch (json::exception& e) {
                                        auto s = el.key() + ": " +
                                            std::string(el.value()[vbase + v]);
                                            throw_json_error(e, s);
                                    }
                                }
                            }
                        }
                    }
                } else if (!param::is_channel_param(el.key())) {
                    /*
                     * If not a parameter (ignore those) log a message
                     */
                    log(log::warning) << "config module " << mod
                                      << " (slot " << module.slot
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
