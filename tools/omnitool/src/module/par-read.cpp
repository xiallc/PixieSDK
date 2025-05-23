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
 * @brief Module Parameter Read command
 */

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
void par_read(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    if (param_opt.empty()) {
        param_opt = chans_opt;
        chans_opt.clear();
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            context.opts.out << "# module param read: " << mod_num
                             << ": " << param_opt << std::endl;
            if (param_opt == "all") {
                for (auto& par : pixie::param::get_module_param_map()) {
                    try {
                        util::io::output_value(
                            context.opts.out, par.first,
                            crate[mod_num].read(par.second));
                    } catch (pixie::error::error& e) {
                        if (e.type != pixie::error::error::code::module_param_disabled &&
                            e.type != pixie::error::error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                util::io::output_value(
                    context.opts.out, param_opt, crate[mod_num].read(param_opt));
            }
        } else {
            pixie::channel::range channels;
            command::channels_option(
                channels, chans_opt, crate[mod_num].num_channels);
            auto ss = context.opts.out.precision(10);
            for (auto channel_num : channels) {
                context.opts.out << "# channel param read: " << mod_num
                                 << ':' << channel_num << ": "
                                 << param_opt << std::endl;
                if (param_opt == "all") {
                    for (auto& par : pixie::param::get_channel_param_map()) {
                        try {
                            util::io::output_value(
                                context.opts.out, par.first, crate[mod_num].read(
                                    par.second, channel_num));
                        } catch (pixie::error::error& e) {
                            if (e.type != pixie::error::error::code::channel_param_disabled &&
                                e.type != pixie::error::error::code::channel_param_writeonly) {
                                throw;
                            }
                        }
                    }
                } else {
                    util::io::output_value(
                        context.opts.out, param_opt,
                        crate[mod_num].read(param_opt, channel_num));
                }
            }
            context.opts.out.precision(ss);
        }
    }
}

void par_read_comp(
    command::context& context, command::completion& completions) {
    auto par_read_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, par_read_cmd.name, 1, completions);

    command::completions::channels_completions(
        context, par_read_cmd.name, 1, 2, completions);

    command::completion_entries module_entries;
    for (auto& par : pixie::param::get_module_param_map()) {
        module_entries.push_back({command::completion_entry::node::argument,
            std::get<0>(par), par_read_cmd.name, "", std::get<0>(par)});
    }

    command::completions::multiargument_completion(
        module_entries, 2, 2, completions);

    if (command::completions::valid_channels_check(context, 1, 2, completions)) {
        command::completion_entries channel_entries;
        for (auto& par : pixie::param::get_channel_param_map()) {
            channel_entries.push_back({command::completion_entry::node::argument,
                std::get<0>(par), par_read_cmd.name, "", std::get<0>(par)});
        }

        command::completions::multiargument_completion(
            channel_entries, 3, 3, completions);
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
