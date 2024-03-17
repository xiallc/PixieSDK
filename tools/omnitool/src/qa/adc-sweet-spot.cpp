/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
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
 * @brief Offset Sweep Test
 */

#include <pixie/pixie16/db/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-threads.hpp>

#include <omnitool-qa.hpp>

namespace xia {
namespace omnitool {
namespace qa {

void adc_sweet_spot(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(
        mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    context.opts.out << "Test: sweet spot : num-modules=" << mod_nums.size()
                     << std::endl;
    pixie::fixture::log_output out =
        [&context](const std::string& s, log::level level) {
            if (level == log::error) {
                context.opts.out << "error: " << s << std::endl;
            } else {
                context.opts.out << s << std::endl;
            }
        };
    for (auto mod_num : mod_nums) {
        if (crate[mod_num].online() &&
            crate[mod_num].fixtures->has_test("adc.sweet-spot")) {
            bool result = false;
            crate[mod_num].fixtures->run_test("adc.sweet-spot", result, out);
        }
    }
}

void adc_sweet_spot_comp(
    command::context& context, command::completion& completions) {
    const std::string cmd = context.cmd.def.name;
    command::completions::modules_completions(context, cmd, 1, completions);
}
}
}
}
