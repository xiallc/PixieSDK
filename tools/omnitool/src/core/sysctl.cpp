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
 * @brief System Control (sysctl) command
 */

#include <pixie/mib.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace command {
void sys_control(context& context) {
    std::string value_opt;
    auto mib_opt = context.cmd.get_arg();
    if (!mib_opt.empty()) {
        auto equals_opt = context.cmd.get_arg();
        if (!equals_opt.empty()) {
            value_opt = context.cmd.get_arg();
            if (equals_opt != "=" || value_opt.empty()) {
                throw std::runtime_error("sysctl: malform MIB set command");
            }
        }
    }
    bool attributes = false;
    xia::mib::mib_walk_func walker =
        [&mib_opt, attributes](xia::mib::node& nod) {
            auto name = nod.name();
            if (mib_opt.empty() ||
                xia::util::string::starts_with(name, mib_opt)) {
                std::cout << name << " = " << nod.str(attributes) << std::endl;
            }
        };
    xia::mib::walk(walker);
}

void sys_control_comp(context& , completion& ) {
}
} // namespace command
} // namespace omnitool
} // namespace xia
