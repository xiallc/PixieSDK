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

#include <pixie_module.hpp>

#include <hw/csr.hpp>
#include <hw/memory.hpp>
#include <hw/run.hpp>

#include <pixie16sys_defs.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace run
{
void
start(module::module& module,
      run_mode mode,
      run_task run_tsk,
      control_task control_tsk)
{
    /*
     * End a run if one is running.
     */
    if (active(module)) {
        end(module);
    }

    /*
     * If this is a new run and it is an acquisition task clear the MCA memory.
     */
    if (mode == run_mode::new_run && run_tsk != run_task::nop) {
        static const size_t block_size = module::max_histogram_length * 4;
        static const size_t mca_end =
            module::max_histogram_length * module.num_channels;
        words zero(block_size);
        memory::mca mca(module);
        for (address addr = 0; addr < mca_end;  addr += block_size) {
            mca.write(addr, zero);
        }
    }

    module.write_var(param::module_var::RunTask,
                     param::value_type(run_tsk));
    module.write_var(param::module_var::ControlTask,
                     param::value_type(control_tsk));
    module.write_var(param::module_var::Resume,
                     param::value_type(mode));

    csr::set(module, 1 << RUNENA);
}

void
end(module::module& module)
{
    csr::clear(module, 1 << RUNENA);
    for (int msecs = 0; msecs < 100; ++msecs) {
        hw::wait(1000);
        if (hw::run::active(module)) {
            return;
        }
    }
    throw error(error::code::module_task_timeout,
                "failed to stop task");
}

bool
active(module::module& module)
{
    return (csr::read(module) & (1 << RUNACTIVE)) != 0;
}

void
control(module::module& module, control_task control_tsk, int wait_msecs)
{
    start(module, run_mode::new_run, run_task::nop, control_tsk);
    for (int msecs = 0; msecs < wait_msecs; ++msecs) {
        hw::wait(1000);
        if (active(module)) {
            return;
        }
    }
    std::ostringstream oss;
    oss << "control task failed to start: " << int(control_tsk);
    throw error(error::code::module_task_timeout, oss.str());
}
}
}
}
}
