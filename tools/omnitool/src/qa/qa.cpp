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
 * @brief QA Tests
 */

#include <omnitool-qa.hpp>

namespace xia {
namespace omnitool {
namespace qa {
omnitool_command_handler_decl(offset_sweep);
static const command::definition offset_sweep_cmd = {
    "QA", "/qa/afe/offset-sweep",
    omnitool_command_handers(offset_sweep),
    {"init", "probe"},
    0, 6, 0,
    {omnitool_command_opt_decl("p"), omnitool_command_opt_arg_decl("s"),
     omnitool_command_opt_arg_decl("o"), omnitool_command_opt_arg_decl("t")},
    "[-p] [-s msec] [-o start,[stop,[step]]] [-t threshold-multiplier] [module(s) [channel(s)]]",
    "Channel offset sweep test, optionally create a plot (-p)"
};

omnitool_command_handler_decl(adc_sweet_spot);
static const command::definition adc_sweet_spot_cmd = {
    "QA", "/qa/afe/adc-sweet-spot",
    omnitool_command_handers(adc_sweet_spot),
    {"init", "probe"},
    0, 1, 0,
    {},
    "[module(s)]",
    "Channel ADC sweet spot test"
};

omnitool_command_handler_decl(mca_mem);
static const command::definition mca_mem_cmd = {
    "QA", "/qa/mca_mem",
    omnitool_command_handers(mca_mem),
    {"init", "probe"},
    0, 5, 0,
    {},
    "[module(s) [legacy] [alias] [bitwalk] [checker]]",
    "MCA Memory tests"
};

static const command::definitions commands = {
    offset_sweep_cmd,
    adc_sweet_spot_cmd,
    mca_mem_cmd
};

void register_commands() {
    command::register_commands(commands);
}
} // namespace qa
} // namespace omnitool
} // namespace xia
