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
 * @brief Crate Firmware Report command
 */

#include <cstring>
#include <fstream>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/reports/reports.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace crate {
void fw_report(command::context& context) {
    auto& crate = context.crate;
    auto file_opt = context.cmd.get_arg();
    if (!file_opt.empty()) {
        std::ofstream out(file_opt);
        if (!out) {
            throw std::runtime_error(
                std::string(
                    "opening report: " + file_opt + ": " + std::strerror(errno)));
        }
        reports::fw_report(crate, out);
    } else {
    }
    crate->output_firmware(context.opts.out);
}

void fw_report_comp(command::context& , command::completion& ) {
}
} // namespace crate
} // namespace omnitool
} // namespace xia
