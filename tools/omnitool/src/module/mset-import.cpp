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
 * @brief Module Setup Import command
 */

#include <pixie/pixie16/crate.hpp>
#include <pixie/config.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void mset_import(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto settings_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    std::string action;
    xia::logging::log_level_guard log_guard(xia::log::debug);
    if (context.cmd.has_arg()) {
        auto action_opt = context.cmd.get_arg();
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " +
                            action_opt));
        }
    }
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        pixie::module::module& module = crate[mod_num];
        pixie::config::import_settings_file(settings_opt, module);
        if (module.online() && !action.empty()) {
            if (action == "flush") {
                module.sync_vars();
            } else if (action == "sync") {
                module.sync_vars();
                module.sync_hw();
            }
        }
    }
}

void mset_import_comp(
    command::context& context, command::completion& completions) {
    auto mset_import_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, mset_import_cmd.name, 1, completions);

    command::completion_entries entries;
    entries.push_back({command::completion_entry::node::argument, "flush",
                        mset_import_cmd.name, "", "flush"});
    entries.push_back({command::completion_entry::node::argument, "sync",
                        mset_import_cmd.name, "", "sync"});

    command::completions::multiargument_completion(
        entries, 3, 3, completions);
}
} // namespace module
} // namespace omnitool
} // namespace xia
