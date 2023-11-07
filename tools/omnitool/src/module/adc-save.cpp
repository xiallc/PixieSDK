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
 * @brief Module ADC TRace Save
 */

#include <fstream>
#include <sstream>

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-defs.hpp>

namespace xia {
namespace omnitool {
namespace module {
void adc_save(command::context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();;
    auto len_opt = context.cmd.get_arg();;
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    size_t length = pixie::hw::max_adc_trace_length;
    if (!len_opt.empty()) {
        length = util::io::get_value<size_t>(len_opt);
    }
    for (auto mod_num : mod_nums) {
        pixie::channel::range channels;
        command::channels_option(
            channels, chans_opt, crate[mod_num].num_channels);
        std::vector<pixie::hw::adc_trace> traces;
        for (auto channel : channels) {
            pixie::hw::adc_trace adc_trace(length);
            crate[mod_num].read_adc(channel, adc_trace, false);
            traces.push_back(adc_trace);
        }
        std::ostringstream name;
        name << std::setfill('0') << omnitool::adc_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < traces.size(); idx++) {
                if (idx != traces.size() - 1)
                    out << traces[idx][bin] << ",";
                else
                    out << traces[idx][bin] << std::endl;
            }
        }
    }
}

void adc_save_comp(
    command::context& context, command::completion& completions) {
    (void) context;
    (void) completions;
}
} // namespace module
} // namespace omnitool
} // namespace xia
