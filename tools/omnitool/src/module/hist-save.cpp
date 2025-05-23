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
 * @brief Module Histogram Save command
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
void hist_save(command::context& context) {
    auto& crate = context.crate;
    auto bins_opt = context.cmd.get_option("-b");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto name_opt = context.cmd.get_arg();
    if (name_opt.empty()) {
        name_opt = chans_opt;
        chans_opt.clear();
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        size_t length = crate[mod_num].channels[0].fixture->config.max_histogram_length;
        if (!bins_opt.empty()) {
            length = util::io::get_value<size_t>(bins_opt);
        }
        std::vector<pixie::hw::words> histos;
        for (auto channel : channels) {
            pixie::hw::words histogram(length);
            crate[mod_num].read_histogram(channel, histogram);
            histos.push_back(histogram);
        }
        std::ostringstream name;
        name << name_opt << '-' << omnitool::histogram_prefix
             << std::setfill('0') << '-' << std::setw(2) << mod_num << ".csv";
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
            for (size_t idx = 0; idx < histos.size(); idx++) {
                if (idx != histos.size() - 1)
                    out << histos[idx][bin] << ",";
                else
                    out << histos[idx][bin] << std::endl;
            }
        }
    }
}

void hist_save_comp(
    command::context& context, command::completion& completions) {
    const std::string cmd = context.cmd.def.name;

    auto not_completed = !command::completions::flag_completion(
        nullptr, cmd, completions);
    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(cmd, completions);
        if (off != 0) {
            command::completions::modules_completions(context, cmd,
                off, completions);

            command::completions::channels_completions(context, cmd,
                off, off + 1, completions);
        }
    }
}

void hist_plot(command::context& context) {
    auto& crate = context.crate;
    auto& out = context.opts.out;
    auto bins_opt = context.cmd.get_option("-b");
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
        size_t length = mod.channels[0].fixture->config.max_histogram_length;
        if (!bins_opt.empty()) {
            length = util::io::get_value<size_t>(bins_opt);
        }
        for (auto chan : channels) {
            pixie::hw::words histogram(length);
            crate[mod_num].read_histogram(chan, histogram);
            if (!histogram.empty()) {
                out << "Module: " << mod.number
                    << " channel: " << chan
                    << std::endl;
                pixie::plot::ascii(histogram, writer, 70, 25);
                out << std::endl;
            }
        }
    }
}

void hist_plot_comp(
    command::context& context, command::completion& completions) {
    const std::string cmd = context.cmd.def.name;

    auto not_completed = !command::completions::flag_completion(
        nullptr, cmd, completions);
    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(cmd, completions);
        if (off != 0) {
            command::completions::modules_completions(context, cmd,
                                                      off, completions);

            command::completions::channels_completions(context, cmd,
                                                       off, off + 1, completions);
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
