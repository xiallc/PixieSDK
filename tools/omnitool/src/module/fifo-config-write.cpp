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
 * @brief Module Worker Write command
 */

#include <pixie/utils/io.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>

namespace xia {
namespace omnitool {
namespace module {
void fifo_config_write(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    auto value = util::io::get_value<pixie::param::value_type>(value_opt);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    for (auto mod_num : mod_nums) {
        auto& mod = crate[mod_num];
        if (param_opt == "bf") {
            throw std::runtime_error("can not write to non-atomic fifo parameter: " + param_opt);
        }
        if (param_opt == "bw") {
            mod.set_fifo_bandwidth(value);
        } else if (param_opt == "dma") {
            mod.set_fifo_dma_trigger_level(value);
        } else if (param_opt == "hold") {
            mod.set_fifo_hold(value);
        } else if (param_opt == "iw") {
            mod.set_fifo_idle_wait(value);
        } else if (param_opt == "rw") {
            mod.set_fifo_run_wait(value);
        } else {
            throw std::runtime_error("invalid fifo config parameter: " + param_opt);
        }
    }
}

void fifo_config_write_comp(
    command::context& context, command::completion& completions) {
    auto fifo_write_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, fifo_write_cmd.name, 1, completions);

    command::completion_entries param_entries;
    param_entries.push_back({command::completion_entry::node::argument, "bw",
                             fifo_write_cmd.name, "", "bw"});
    param_entries.push_back({command::completion_entry::node::argument, "dma",
                             fifo_write_cmd.name, "", "dma"});
    param_entries.push_back({command::completion_entry::node::argument, "hold",
                             fifo_write_cmd.name, "", "hold"});
    param_entries.push_back({command::completion_entry::node::argument, "iw",
                             fifo_write_cmd.name, "", "iw"});
    param_entries.push_back({command::completion_entry::node::argument, "rw",
                             fifo_write_cmd.name, "", "rw"});

    command::completions::multiargument_completion(
        param_entries, 2, 2, completions);
}
}
}
}
