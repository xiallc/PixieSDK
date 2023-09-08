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
 * @brief Module Parameter Write command
 */

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool/commands.hpp>

namespace xia {
namespace omnitool {
namespace module {
void par_write(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    if (value_opt.empty()) {
        value_opt = param_opt;
        param_opt = chans_opt;
        chans_opt.clear();
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    const auto value = util::io::get_value<double>(value_opt);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            bool bcast = crate[mod_num].write(param_opt, pixie::param::value_type(value));
            if (bcast) {
                pixie::crate::crate::user user(crate);
                for (size_t m = 0; m < crate.num_modules; ++m) {
                    if (mod_num != m) {
                        auto& module = crate[m];
                        if (module.online()) {
                            module.write(param_opt, pixie::param::value_type(value));
                        }
                    }
                }
                break;
            }
        } else {
            pixie::channel::range channels;
            command::channels_option(
                channels, chans_opt, crate[mod_num].num_channels);
            auto& module = crate[mod_num];
            for (auto channel_num : channels) {
                module.write(param_opt, channel_num, value);
            }
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
