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
 * @brief Module Variable Read command
 */

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool/commands.hpp>

namespace xia {
namespace omnitool {
namespace module {
  using error = xia::pixie::error::error;

void var_read(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto offsets_opt = context.cmd.get_arg();
    if (!util::string::check_number_range(chans_opt)) {
        param_opt = chans_opt;
        chans_opt.clear();
    }
    if (offsets_opt.empty()) {
        offsets_opt = "0";
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        auto& mod = crate[mod_num];
        if (chans_opt.empty()) {
            const auto& desc =
                pixie::param::lookup_module_descriptor(
                    param_opt, mod.module_var_descriptors);
            auto offsets =
                util::io::get_values<size_t>(offsets_opt, desc.size);
            if (param_opt == "all") {
                context.opts.out << "# module var read: " << mod_num << ": "
                                 << param_opt << std::endl;
                for (auto& var : mod.module_var_descriptors) {
                    for (auto offset : offsets) {
                        try {
                            util::io::output_value(
                                context.opts.out, var.name,
				mod.read_var(var.par, offset));
                        } catch (error& e) {
                            if (e.type != error::code::module_param_disabled &&
                                e.type != error::code::module_param_writeonly) {
                                throw;
                            }
                        }
                    }
                }
            } else {
                for (auto offset : offsets) {
                    util::io::output_value(
                        context.opts.out, param_opt,
			mod.read_var(param_opt, 0, offset));
                }
            }
        } else {
            pixie::channel::range channels;
            command::channels_option(channels, chans_opt, mod.num_channels);
            for (auto channel : channels) {
                const auto& desc =
                    pixie::param::lookup_channel_descriptor(
                        param_opt, mod.channel_var_descriptors);
                auto offsets =
                    util::io::get_values<size_t>(offsets_opt, desc.size);
                if (param_opt == "all") {
                    context.opts.out << "# channel var read: " << mod_num << ':'
                                     << channel << ": " << param_opt << std::endl;
                    for (auto& var : mod.channel_var_descriptors) {
                        for (auto offset : offsets) {
                            try {
                                util::io::output_value(
                                    context.opts.out, var.name, mod.read_var(
                                        var.par, channel, offset));
                            } catch (error& e) {
                                if (e.type != error::code::channel_param_disabled &&
                                    e.type != error::code::channel_param_writeonly) {
                                    throw;
                                }
                            }
                        }
                    }
                } else {
                    for (auto offset : offsets) {
                        util::io::output_value(
                            context.opts.out, param_opt, mod.read_var(
                                param_opt, channel, offset));
                    }
                }
            }
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
