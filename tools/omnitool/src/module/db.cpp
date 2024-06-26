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
 * @brief Module Daughter Board command
 */

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/reports/reports.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void db(command::context& context) {
    auto check_cmd = [](auto& str) {
      return str == "show" ||
        str == "disable-swap";
    };
    auto& crate = context.crate;
    command::argument mod_nums_opt;
    command::argument chans_opt;
    auto db_opt = context.cmd.get_arg();
    if (!check_cmd(db_opt)) {
        mod_nums_opt = db_opt;
        db_opt = context.cmd.get_arg();
    }
    if (!check_cmd(db_opt)) {
        chans_opt = db_opt;
        db_opt = context.cmd.get_arg();
    }
    std::string db_command = "show";
    if (!db_opt.empty()) {
        if (!check_cmd(db_opt)) {
            throw std::runtime_error("db: invalid command: " + db_opt);
        }
        db_command = db_opt;
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        if (crate[mod_num] < pixie::hw::rev_H) {
            throw std::runtime_error(
                "db: module not rev H or later: " + std::to_string(mod_num));
        }
        if (!crate[mod_num].online()) {
            throw std::runtime_error("db: module off-line");
        }
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        for (auto channel : channels) {
            auto& fixture = crate[mod_num].channels[channel].fixture;
            if (!fixture) {
                throw std::runtime_error(
                    "db: no fixture: module: " + std::to_string(mod_num) +
                    " channel: " + std::to_string(channel));
            }
            if (db_command == "show") {
                context.opts.out << "DB: module:" << mod_num
                                 << " channel:" << channel << std::endl;
                reports::report(*fixture, context.opts.out, " ");
            } else if (db_command == "disable-swap") {
                fixture->set("ADC_SWAP_DISABLE", true);
            }
        }
    }
}

void db_comp(
    command::context& context, command::completion& completions) {
    auto db_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, db_cmd.name, 1, completions);

    command::completions::channels_completions(
        context, db_cmd.name, 1, 2, completions);

    command::completions::argument_completion(
        {command::completion_entry::node::argument, "disable-swap",
        db_cmd.name, "", "disable-swap"},
        2, 3, completions);
}
} // namespace module
} // namespace omnitool
} // namespace xia
