/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

/**
 * @brief Module ADC TRace Save & Plotting
 */

#include <fstream>
#include <sstream>

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/plot.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-defs.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void adc_save(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto len_opt = context.cmd.get_arg();
    auto name_opt = context.cmd.get_arg();
    if (name_opt.empty()) {
        name_opt = len_opt;
        len_opt.clear();
        if (name_opt.empty()) {
            name_opt = chans_opt;
            chans_opt.clear();
            if (name_opt.empty()) {
                name_opt = mod_nums_opt;
                mod_nums_opt.clear();
            }
        }
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    size_t length = pixie::hw::max_adc_trace_length;
    if (!len_opt.empty()) {
        length = util::io::get_value<size_t>(len_opt);
    }
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        std::vector<pixie::hw::adc_trace> traces;
        for (auto channel : channels) {
            pixie::hw::adc_trace adc_trace(length);
            crate[mod_num].read_adc(channel, adc_trace, false);
            traces.push_back(adc_trace);
        }
        std::ostringstream name;
        name << name_opt << '-' << std::setfill('0') << omnitool::adc_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < traces.size(); idx++) {
                if (idx != traces.size() - 1)
                    out << traces[idx][bin] << ",";
                else
                    out << traces[idx][bin] << std::endl;
            }
        }
    }
}

void adc_save_comp(
    command::context& context, command::completion& completions) {
    auto adc_save_cmd = context.cmd.def;

    command::completions::modules_completions(context, adc_save_cmd.name,
        1, completions);

    command::completions::channels_completions(context, adc_save_cmd.name,
        1, 2, completions);
}

void adc_plot(command::context& context) {
    auto& crate = context.crate;
    auto& out = context.opts.out;
    auto acquire_opt = context.cmd.get_option("-a");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    pixie::plot::write_string_func writer =
        [&out](const std::string& s) { out << s; };
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        auto& mod = crate[mod_num];
        if (acquire_opt == "true") {
            mod.get_traces();
        }
        for (auto chan : channels) {
            auto& channel = mod.channels[chan];
            if (!channel.adc_trace.empty()) {
                out << "Module: " << mod.number
                    << " channel: " << channel.number
                    << std::endl;
                pixie::plot::ascii(channel.adc_trace, writer, 70, 25);
                out << std::endl;
            }
        }
    }
}

void adc_plot_comp(
    command::context& context, command::completion& completions) {
    auto adc_plot_cmd = context.cmd.def;

    auto not_completed = !command::completions::flag_completion(
        nullptr, adc_plot_cmd.name, completions);
    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(
            adc_plot_cmd.name, completions);
        if (off != 0) {
            command::completions::modules_completions(context,
                                                      adc_plot_cmd.name,
                                                      off, completions);

            command::completions::channels_completions(context,
                                                       adc_plot_cmd.name,
                                                       off, off + 1,
                                                       completions);
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
