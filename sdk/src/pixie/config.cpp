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

#include <pixie/config.hpp>
#include <pixie/param.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/hw.hpp>

#include <nolhmann/json.hpp>

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
import_json(const std::string& filename,
            crate::crate& crate,
            module::number_slots& loaded)
{
    std::ifstream input_json(filename);
    if (!input_json) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json config: " + filename +
                    ": " + std::strerror(errno));
    }

    json config;

    try {
        config = json::parse(input_json);
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
                auto slot = metadata["slot"];
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
             * Write the config to the module variables
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
             * Write the config to the channel variables
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

static json
json_firmware(const firmware::firmware_ref fw)
{
    json jfw;
    jfw["tag"] = fw->tag;
    jfw["file"] = fw->basename();
    jfw["version"] = fw->version;
    jfw["rev"] = fw->mod_revision;
    jfw["adc_msps"] = fw->mod_adc_msps;
    jfw["adc_bits"] = fw->mod_adc_bits;
    return jfw;
}

void
export_json(const std::string& filename, crate::crate& crate)
{
    json config;

    for (auto m : crate.modules) {
        module::module& mod = *m;

        json metadata;
        char rv[2] = { mod.revision_label(), '\0' };
        metadata["number"] = mod.number;
        metadata["slot"] = mod.slot;
        metadata["serial-num"] = mod.serial_num;
        metadata["hardware_revision"] = rv;
        metadata["num-channels"] = mod.num_channels;
        metadata["sys"] = json_firmware(mod.get("sys"));
        metadata["fippi"] = json_firmware(mod.get("fippi"));
        metadata["dsp"] = json_firmware(mod.get("dsp"));
        metadata["var"] = json_firmware(mod.get("var"));
        metadata["fifo"]["buffers"] = mod.fifo_buffers;
        metadata["fifo"]["run-wait"] = mod.fifo_run_wait_usecs.load();
        metadata["fifo"]["idle-wait"] = mod.fifo_idle_wait_usecs.load();
        metadata["fifo"]["hold"] = mod.fifo_hold_usecs.load();
        metadata["config"] = json::array();
        for (auto& chan : mod.channels) {
            json cfg;
            cfg["adc_bits"] = chan.config.adc_bits;
            cfg["adc_msps"] = chan.config.adc_msps;
            cfg["adc_clk_div"] = chan.config.adc_clk_div;
            cfg["fpga_clk_mhz"] = chan.config.fpga_clk_mhz;
            metadata["config"].push_back(cfg);
        }

        json module;
        for (auto& var : mod.module_vars) {
            auto& desc = var.var;
            if (desc.mode != param::ro) {
                if (desc.size == 1) {
                    module[desc.name] = var.value[0].value;
                } else {
                    json value;
                    for (auto v : var.value) {
                        value.push_back(v.value);
                    }
                    module[desc.name] = value;
                }
            }
        }

        json channel;
        for (auto& desc : mod.channel_var_descriptors) {
            json values;
            for (auto& chan : mod.channels) {
                for (auto& v : chan.vars[int(desc.par)].value) {
                    values.push_back(v.value);
                }
            }
            channel[desc.name] = values;
        }

        json mod_config;
        mod_config["metadata"] = metadata;
        mod_config["module"] = { { "input" , module } };
        mod_config["channel"] = { { "input", channel } };

        config.push_back(mod_config);
    }

    std::ofstream output_json(filename);
    if (!output_json) {
        throw error(pixie::error::code::file_open_failure,
                    "opening json config: " + filename +
                    ": " + std::strerror(errno));
    }

    output_json << std::setw(4) << config << std::endl;
}

}
}
}
