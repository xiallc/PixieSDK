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
 * @brief Module Legacy Settings Import command
 */

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace module {
void lset_import(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto settings_opt = context.cmd.get_arg();
    auto action_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    std::string action;
    if (!action_opt.empty()) {
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " + action_opt));
        }
    }
    for (auto mod_num : mod_nums) {
        pixie::module::module& module = crate[mod_num];
        pixie::legacy::settings settings(module);
        settings.load(settings_opt);
        settings.import(module);
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

bool lset_import_comp(
    command::context& context, command::command_completion& completions) {
    (void) context;
    (void) completions;
    return false;
}
} // namespace module
} // namespace omnitool
} // namespace xia
