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
 * @brief Crate Export command
 */

#include <pixie/utils/time.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace crate {
void export_(command::context& context) {
    auto& crate = context.crate;
    auto file_opt = context.cmd.get_arg();
    util::time::timepoint tp;
    tp.start();
    crate->export_config(file_opt);
    tp.end();
    context.opts.out << "Modules export time=" << tp << std::endl;
}

bool export__comp(
    command::context& context, command::command_completion& completions) {
    (void) context;
    (void) completions;
    return false;
}
} // namespace crate
} // namespace omnitool
} // namespace xia
