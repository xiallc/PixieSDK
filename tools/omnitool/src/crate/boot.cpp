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
 * @brief Crate command
 */

#include <pixie/utils/io.hpp>
#include <pixie/utils/time.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-crate.hpp>

namespace xia {
namespace omnitool {
namespace crate {
void boot(command::context& context) {
    auto& crate = context.crate;
    pixie::crate::crate::boot_params boot_params;
    bool first = true;
    bool clear_boots = true;
    while (context.cmd.has_arg()) {
        auto opt = context.cmd.get_arg();
        if (opt.empty()) {
            break;
        }
        if (first) {
            first = false;
            using range_type = pixie::crate::crate::boot_params::range_type;
            boot_params.slots =
                util::io::get_values<range_type>(opt, crate.num_modules);
            if (!boot_params.slots.empty()) {
                crate.modules_to_slots(boot_params.slots);
                continue;
            }
        }
        if (clear_boots) {
            clear_boots = false;
            boot_params.boot_comms = false;
            boot_params.boot_fippi = false;
            boot_params.boot_dsp = false;
        }
        if (opt == "comms" || opt == "sys" || opt == "system") {
            boot_params.boot_comms = true;
        } else if (opt == "fippi") {
            boot_params.boot_fippi = true;
        } else if (opt == "dsp") {
            boot_params.boot_dsp = true;
        } else {
            throw std::runtime_error("boot: invalid option: " + opt);
        }
    }
    util::time::timepoint tp;
    if (context.opts.verbose) {
        bool dsp_boot_forced = boot_params.boot_comms || boot_params.boot_fippi;
        context.opts.out << std::boolalpha
                         << "booting crate: comms=" << boot_params.boot_comms
                         << " fippi=" << boot_params.boot_fippi
                         << " dsp=" << boot_params.boot_dsp
                         << " (forced=" << dsp_boot_forced
                         << ") slots";
        if (boot_params.slots.empty()) {
            context.opts.out << "=all";
        } else {
            char delimiter = '=';
            for (auto m : boot_params.slots) {
                context.opts.out << delimiter << m;
                delimiter = ',';
            }
        }
        context.opts.out << std::endl;
        tp.start();
    }
    crate->boot(boot_params);
    if (context.opts.verbose) {
        tp.end();
        context.opts.out << "boot time=" << tp << std::endl;
    }
}

void boot_comp(
    command::context& context, command::completion& completions) {
    auto boot_cmd = context.cmd.def;

    command::completions::modules_completions(context, boot_cmd.name,
        1, completions);

    command::completions::argument_completion(
        {command::completion_entry::node::argument, "comms", boot_cmd.name, "", "comms"},
        2, 4, completions);

    command::completions::argument_completion(
        {command::completion_entry::node::argument, "fippi", boot_cmd.name, "", "fippi"},
        2, 4, completions);

    command::completions::argument_completion(
        {command::completion_entry::node::argument, "dsp", boot_cmd.name, "", "dsp"},
        2, 4, completions);
}
} // namespace crate
} // namespace omnitool
} // namespace xia
