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
 * @brief Module Copy Parameters command
 */

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace module {
void copy(command::context& context) {
    auto& crate = context.crate;
    command::command::argument filter_mask;
    command::command::argument src_mod_opt;
    command::command::argument dest_mod_opt;
    command::command::argument src_chan_opt;
    command::command::argument dest_chan_opt;
    filter_mask = context.cmd.get_arg();
    src_mod_opt = context.cmd.get_arg();
    dest_mod_opt = context.cmd.get_arg();
    if (context.cmd.has_arg()) {
        src_chan_opt = context.cmd.get_arg();
        dest_chan_opt = context.cmd.get_arg();
    }
    command::module_range src_mod_nums;
    command::modules_option(src_mod_nums, src_mod_opt, crate.num_modules);
    command::module_range dest_mod_nums;
    command::modules_option(
        dest_mod_nums, dest_mod_opt, crate.num_modules);
    if (src_mod_nums.size() > 1 || dest_mod_nums.size() > 1) {
        throw std::runtime_error("can only copy from one module to one module");
    }
    std::string string_mask = filter_mask;
    int mask = util::io::get_value<int>(string_mask);
    if (src_chan_opt.empty() || dest_chan_opt.empty()) {
        pixie::param::copy_parameters(
            mask, crate[src_mod_nums[0]].module_vars,
            crate[dest_mod_nums[0]].module_vars);
    } else {
        pixie::channel::range src_chan_nums;
        command::channels_option(
            src_chan_nums, src_chan_opt, crate[src_mod_nums[0]].num_channels);
        pixie::channel::range dest_chan_nums;
        command::channels_option(
            dest_chan_nums, dest_chan_opt, crate[dest_mod_nums[0]].num_channels);
        pixie::param::copy_parameters(
      mask, crate[src_mod_nums[0]][src_chan_nums[0]].vars,
      crate[dest_mod_nums[0]][dest_chan_nums[0]].vars,
      crate[src_mod_nums[0]].module_vars,
      crate[dest_mod_nums[0]].module_vars);
    }
    crate[dest_mod_nums[0]].sync_vars();
}
} // namespace module
} // namespace omnitool
} // namespace xia
