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
 * @brief Module Baseline Save command
 */

#include <fstream>
#include <sstream>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-defs.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void bl_save(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        pixie::channel::baseline::channels_values
            baselines(crate[mod_num].num_channels);
        crate[mod_num].bl_get(channels, baselines, false);

        std::ostringstream name;
        name << std::setfill('0') << omnitool::baseline_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "sample, time,";

        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }

        for (unsigned int sample = 0; sample < baselines.front().size(); sample++) {
            out << sample << "," << std::get<0>(baselines.front()[sample]) << ",";
            for (unsigned int idx = 0; idx < channels.size(); idx++) {
                if (idx != channels.size() - 1)
                    out << std::get<1>(baselines[channels[idx]][sample]) << ",";
                else
                    out << std::get<1>(baselines[channels[idx]][sample]) << std::endl;
            }
        }
    }
}

void bl_save_comp(
    command::context& context, command::completion& completions) {
    auto bl_save_cmd = context.cmd.def;

    command::completions::modules_completions(context, bl_save_cmd.name,
        1, completions);

    command::completions::channels_completions(context, bl_save_cmd.name,
            1, 2, completions);
}
} // namespace module
} // namespace omnitool
} // namespace xia
