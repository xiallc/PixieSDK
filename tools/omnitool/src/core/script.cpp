/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2025 XIA LLC, All rights reserved.
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
 * @brief Command to run omni script
 */

#include <regex>

#include <pixie/mib.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace command {
void script(context& context) {
    if (context.opts.command_depth > 3) {
        throw std::runtime_error(
            "script: recursion limit reached");
    }
    auto file_opt = context.cmd.get_arg();
    xia::omnitool::command::arguments cmds;
    xia::omnitool::command::load_commands(file_opt, cmds);
    xia::omnitool::command::batch batch;
    batch.parse(cmds, context.opts.path);
    batch.execute(context.crate, context.opts);
}

void script_comp(context& , completion& ) {
}
}
}
}