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
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
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
        name << omnitool::histogram_prefix
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

    command::completions::flag_handler flags_func =
        [&context, &completions, &cmd](auto flag) {
        if (flag == "-b") {
            completions.add({command::completion_entry::node::argument,
                            context.cmd.def.name, context.cmd.def.group,
                            context.cmd.def.help_cmd, context.cmd.def.name});
            completions.add(
                {command::completion_entry::node::argument, "-b", cmd, "bins", "-b"});
        }
    };

    auto not_completed = command::completions::flag_completion(
        flags_func, cmd, completions);
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
