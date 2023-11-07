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
 * @brief Module Statistics command
 */

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace module {
void stats(command::context& context) {
    auto& crate = context.crate;
    auto stat_opt = context.cmd.get_option("-s");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    std::string stat = "all";
    if (!stat_opt.empty()) {
        if (stat_opt == "all" ||
            stat_opt == "pe" || stat_opt == "ocr" ||
            stat_opt == "rt" || stat_opt == "lt") {
            stat = stat_opt;
        } else {
            throw std::runtime_error("invalid stat type: " + stat_opt);
        }
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        pixie::stats::stats stats(crate[mod_num]);
        crate[mod_num].read_stats(stats);
        if (stat == "all" || stat == "pe") {
            context.opts.out << "module " << mod_num
                             << ": processed-events="
                             << stats.mod.processed_events() << std::endl;
        }
        if (stat == "all" || stat == "icr") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": input-count-rate="
                                 << stats.chans[channel].input_count_rate()
                                 << std::endl;
            }
        }
        if (stat == "all" || stat == "ocr") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": output-count-rate="
                                 << stats.chans[channel].output_count_rate()
                                 << std::endl;
            }
        }
        if (stat == "all" || stat == "rt") {
            context.opts.out << "module " << mod_num
                             << ": real-time=" << stats.mod.real_time()
                             << std::endl;
        }
        if (stat == "all" || stat == "lt") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": live-time="
                                 << stats.chans[channel].live_time() << std::endl;
            }
        }
    }
}

void stats_comp(
    command::context& context, command::completion& completions) {
    (void) context;
    (void) completions;
}
} // namespace module
} // namespace omnitool
} // namespace xia
