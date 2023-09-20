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
 * @brief Crate Report command
 */

#include <cstring>
#include <fstream>

#include <pixie/format.hpp>
#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/reports/reports.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>

namespace xia {
namespace omnitool {
namespace crate {
void report(command::context& context) {
    auto& crate = context.crate;
    auto json_opt = context.cmd.get_option("-j");
    std::ostream* out= &context.opts.out;
    std::ofstream output_file;
    auto file_opt = context.cmd.get_arg();
    auto mod_nums_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    if (mod_nums.empty()) {
        xia::log(log::warning) << "report: crate: no present modules provided";
        return;
    }
    if (!file_opt.empty()) {
        output_file.open(file_opt);
        if (!output_file) {
            throw std::runtime_error(
                std::string(
                    "opening report: " + file_opt + ": " + std::strerror(errno)));
        }
        out = &output_file;
    }
    if (json_opt == "true") {
        std::string report_str;
        reports::report(crate, report_str, mod_nums);
        *out << report_str;
    } else {
        reports::report(crate, *out, mod_nums);
    }
}

void report_comp(command::context& context, command::completion& completions) {
    auto report_cmd = context.cmd.def;

    auto not_completed = !command::completions::flag_completion(NULL, report_cmd.name,
                                                                completions);

    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(report_cmd.name, completions);
        if (off != 0) {
            command::completions::modules_completions(context, report_cmd.name, off + 1,
                                                      completions);
        }
    }
}
} // namespace crate
} // namespace omnitool
} // namespace xia
