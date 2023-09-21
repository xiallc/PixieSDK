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
 * @brief Module Worker Read command
 */

#include <pixie/utils/io.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>

namespace xia {
namespace omnitool {
namespace module {
void fifo_config_read(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    std::string par = "all";
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        auto& mod = crate[mod_num];
        if (!param_opt.empty()) {
            if (param_opt == "all" || param_opt == "bw" ||
                param_opt == "bf" || param_opt == "dma" ||
                param_opt == "iw" || param_opt == "rw") {
                par = param_opt;
            } else {
                throw std::runtime_error("invalid fifo config parameter: " + param_opt);
            }
        }
        if (par == "all" || par == "bw") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "bandwidth", mod.fifo_bandwidth.load());
        }
        if (par == "all" || par == "bf") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "buffers", mod.fifo_buffers);
        }
        if (par == "all" || par == "dma") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "dma trigger", mod.fifo_dma_trigger_level.load());
        }
        if (par == "all" || par == "hold") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "hold (usecs)", mod.fifo_hold_usecs.load());
        }
        if (par == "all" || par == "iw") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "idle wait (usecs)", mod.fifo_idle_wait_usecs.load());
        }
        if (par == "all" || par == "rw") {
            context.opts.out << "slot " << mod.slot << ": ";
            util::io::output_value(context.opts.out, "run wait (usecs)", mod.fifo_run_wait_usecs.load());
        }
        context.opts.out << std::endl;
    }
}

void fifo_config_read_comp(
    command::context& context, command::completion& completions) {
    auto fifo_read_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, fifo_read_cmd.name, 1, completions);

    command::completion_entries param_entries;
    param_entries.push_back({command::completion_entry::node::argument, "all",
                             fifo_read_cmd.name, "", "all"});
    param_entries.push_back({command::completion_entry::node::argument, "bw",
                             fifo_read_cmd.name, "", "bw"});
    param_entries.push_back({command::completion_entry::node::argument, "bf",
                             fifo_read_cmd.name, "", "bf"});
    param_entries.push_back({command::completion_entry::node::argument, "dma",
                             fifo_read_cmd.name, "", "dma"});
    param_entries.push_back({command::completion_entry::node::argument, "hold",
                             fifo_read_cmd.name, "", "hold"});
    param_entries.push_back({command::completion_entry::node::argument, "iw",
                             fifo_read_cmd.name, "", "iw"});
    param_entries.push_back({command::completion_entry::node::argument, "rw",
                             fifo_read_cmd.name, "", "rw"});

    command::completions::multiargument_completion(
        param_entries, 2, 2, completions);
}
}
}
}