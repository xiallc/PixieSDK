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

#include <pixie_log.hpp>
#include <pixie_module.hpp>
#include <pixie_util.hpp>

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
static const char*
control_task_labels(control_task control_tsk)
{
    switch (control_tsk) {
    case control_task::set_dacs:
        return "set_dacs";
    case control_task::enable_input:
        return "enable_input";
    case control_task::ramp_offsetdacs:
        return "ramp_offsetdacs";
    case control_task::get_traces:
        return "get_traces";
    case control_task::program_fippi:
        return "program_fippi";
    case control_task::get_baselines:
        return "get_baselines";
    case control_task::adjust_offsets:
        return "adjust_offsets";
    case control_task::tau_finder:
        return "tau_finder";
    case control_task::reset_adc:
        return "reset_adc";
    default:
        break;
    }
    return "nop";
};

static const char* run_mode_labels[] =
{
    "resume",
    "new_run"
};

static const char*
run_task_labels(run_task run_tsk)
{
    switch (run_tsk) {
    case run_task::list_mode:
        return "list_mode";
    case run_task::histogram:
        return "hostogram";
    default:
        break;
    }
    return "nop";
}
void
start(module::module& module,
      run_mode mode,
      run_task run_tsk,
      control_task control_tsk)
{
    log(log::debug) << module::module_label(module, "run")
                    << "start: run-mode=" << int(mode)
                    << " run-tsk=" << std::hex << int(run_tsk) << std::dec
                    << " control-tsk=" << int(control_tsk);

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
        static const size_t block_size = max_histogram_length * 4;
        static const size_t mca_end =
            max_histogram_length * module.num_channels;
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
    log(log::debug) << module::module_label(module, "run") << "end";
    util::timepoint tp;
    tp.start();
    csr::clear(module, 1 << RUNENA);
    for (int msecs = 0; msecs < 2 * 100; ++msecs) {
        if (!hw::run::active(module)) {
            tp.end();
            log(log::debug) << module::module_label(module, "run")
                            << "end duration=" << tp;
            return;
        }
        hw::wait(500);
    }
    throw error(error::code::module_task_timeout,
                "failed to stop task");
}

bool
active(module::module& module)
{
    return (csr::read(module) & ((1 << RUNENA) | (1 << RUNACTIVE))) != 0;
}

void
control(module::module& module, control_task control_tsk, int wait_msecs)
{
    log(log::debug) << module::module_label(module, "run")
                    << "control=" << control_task_labels(control_tsk)
                    << " wait=" << wait_msecs;
    util::timepoint tp;
    tp.start();
    wait_msecs *= 2;
    start(module, run_mode::new_run, run_task::nop, control_tsk);
    for (int msecs = 0; msecs < wait_msecs; ++msecs) {
        if (!active(module)) {
            tp.end();
            log(log::debug) << module::module_label(module, "run")
                            << "control=" << control_task_labels(control_tsk)
                            << " duration=" << tp;
            return;
        }
        hw::wait(500);
    }
    std::ostringstream oss;
    oss << "control task failed to start: " << int(control_tsk);
    throw error(error::code::module_task_timeout, oss.str());
}

void
run(module::module& module, run_mode mode, run_task run_tsk)
{
    log(log::debug) << module::module_label(module, "run")
                    << "mode=" << run_mode_labels[int(mode)]
                    << " run=" << run_task_labels(run_tsk);
    start(module, mode, run_tsk, control_task::nop);
}
}
}
}
}
