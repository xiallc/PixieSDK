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
 * @brief Module Override Default Firmware command
 */

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void override_fw(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto fwfile_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    if (mod_nums.size() > 1) {
        throw std::runtime_error("override_report: too many modules");
    }
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        pixie::firmware::override_default_fw(
            crate[mod_num].firmware, fwfile_opt);
    }
}

void override_fw_comp(
    command::context& context, command::completion& completions) {
    command::completions::modules_completions(context,
       context.cmd.def.name, 1, completions);
}
} // namespace module
} // namespace omnitool
} // namespace xia
