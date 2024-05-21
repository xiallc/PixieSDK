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

#include <regex>

#include <pixie/mib.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace command {
void sys_control(context& context) {
    auto json_opt = context.cmd.get_option("-j");
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
    std::regex mib_match(mib_opt);
    bool attributes = false;
    bool json_mode = json_opt == "true";
    xia::pixie::format::json json_out;
    xia::mib::mib_walk_func walker =
        [&mib_opt, &value_opt, &mib_match, attributes, json_mode, &json_out](xia::mib::node& nod) {
            auto name = nod.name();
            if (mib_opt.empty() || std::regex_search(name, mib_match)) {
                if (!value_opt.empty()) {
                    nod.set_value(value_opt);
                }
                if (json_mode) {
                    mib::mib_to_json(json_out, nod.str(attributes), name);
                } else {
                    std::cout << name << " = " << nod.str(attributes) << std::endl;
                }
            }
        };
    xia::mib::walk(walker);
    if (json_mode) {
        std::cout << json_out.dump(4) << std::endl;
    }
}

void sys_control_comp(context& , completion& ) {
}
} // namespace command
} // namespace omnitool
} // namespace xia
