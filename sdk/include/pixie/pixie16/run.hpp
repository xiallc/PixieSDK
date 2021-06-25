#ifndef PIXIE_HW_RUN_H
#define PIXIE_HW_RUN_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

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
    list_mode = 0x101,
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
