/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file run.hpp
 * @brief Defines run control enumerators and functions for the Pixie-16 hardware.
 */

#ifndef PIXIE_HW_RUN_H
#define PIXIE_HW_RUN_H

#include <cstdint>

#include <pixie/pixie16/hw.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
class module;
}
namespace hw
{
namespace run
{
/*
 * Run and control settings
 */
enum struct run_mode {
    new_run = 1,
    resume = 0
};

enum struct run_task {
    nop = 0,
    list_mode = 0x100,
    histogram = 0x301
};

enum struct control_task {
    set_dacs = 0,
    enable_input = 1,
    ramp_offsetdacs = 3,
    get_traces = 4,
    program_fippi = 5,
    get_baselines = 6,
    adjust_offsets = 7,
    tau_finder = 8,
    fill_ext_fifo = 11,
    reset_adc = 23,
    nop = 100
};

/*
 * Run and control task management.
 */
void start(module::module& module,
           run_mode mode,
           run_task run_tsk,
           control_task control_tsk);
void end(module::module& module);
bool active(module::module& module);

/*
 * Control task
 */
void control(module::module& module,
             control_task control_tsk,
             int wait_msecs = 10000);

/*
 * Run task
 */
void run(module::module& module, run_mode mode, run_task run_tsk);
}
}
}
}

#endif  // PIXIE_HW_RUN_H
