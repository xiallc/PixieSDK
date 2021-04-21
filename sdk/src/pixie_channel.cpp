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

#include <cmath>

#include <pixie_channel.hpp>
#include <pixie_module.hpp>
#include <pixie_util.hpp>

#include <hw/memory.hpp>
#include <hw/run.hpp>

namespace xia
{
namespace pixie
{
namespace channel
{
/*
 * System FPGA clock frequency in MHZ
 */
static const size_t SYSTEM_CLOCK_MHZ = 100;

/*
 * Length of each baseline (default: 2 timestamp words + 16 baselines)
 */
static const size_t BASELINES_BLOCK_LEN = 18;

fifo::fifo(module::module& module_)
    : module(module_)
{
}

void
fifo::update(const size_t channel, param::value_type trace_delay)
{
    const param::value_type slow_filter_range =
        1 << module.read_var(param::module_var::SlowFilterRange, 0, false);
    const param::value_type fast_filter_range =
        1 << module.read_var(param::module_var::FastFilterRange, 0, false);
    const param::value_type fifo_length =
        module.read_var(param::module_var::FIFOLength, 0, false);
    const param::value_type peak_sep =
        module.read_var(param::channel_var::PeakSep, channel, 0, false);

    param::value_type trigger_delay;
    param::value_type paf_length;

    /*
     * Calulate the trigger delay and PAF length.
     */
    trigger_delay = (peak_sep - 1) * slow_filter_range;
    paf_length = (trigger_delay / fast_filter_range) + trigger_delay;

    /*
     * PAF Length must be larger than the FIFO Length
     */
    if (paf_length > fifo_length) {
        paf_length = fifo_length - 1;
        trigger_delay = (paf_length - trace_delay) * fast_filter_range;
    }

    module.write_var(param::channel_var::TriggerDelay,
                     channel,
                     trigger_delay);
    module.write_var(param::channel_var::PAFlength,
                     channel,
                     paf_length);
}

baseline::baseline(module::module& module_, size_t num)
    : module(module_),
      values(num)
{
}

void
baseline::find_cut(const size_t channel)
{
    param::value_type log2_bweight =
        module.read_var(param::channel_var::Log2Bweight, channel, 0, false);
    param::value_type current_bl_cut =
        module.read_var(param::channel_var::BLcut, channel, 0, false);

    module.write_var(param::channel_var::Log2Bweight, channel, 0);
    module.write_var(param::channel_var::BLcut, channel, 0);

    try {
        param::value_type bl_cut;

        bl_cut = compute_cut(channel);
        module.write_var(param::channel_var::BLcut, channel, bl_cut);
        bl_cut = compute_cut(channel);
        module.write_var(param::channel_var::BLcut, channel, bl_cut);
    } catch (...) {
        try {
            module.write_var(param::channel_var::Log2Bweight,
                             channel,
                             log2_bweight);
            module.write_var(param::channel_var::Log2Bweight,
                             channel,
                             current_bl_cut);
        } catch (...) {
            /* ignore nesting exceptions, keep the first */
        }
        throw;
    }

    module.write_var(param::channel_var::Log2Bweight, channel, log2_bweight);
}

double
baseline::time(hw::word time_word0, hw::word time_word1)
{
    double time;
    time = double(static_cast<uint64_t>(time_word0) << 32);
    time += double(time_word1);
    time *= 1.0e-6 / double(SYSTEM_CLOCK_MHZ);
    return time;
}

void
baseline::get(size_t channel)
{
    hw::memory::dsp dsp(module);
    hw::io_buffer buffer;

    hw::run::control(module, hw::run::control_task::get_baselines);

    dsp.read(hw::memory::IO_BUFFER_ADDR, buffer);

    double starttime = time(buffer[0], buffer[1]);

    for (size_t bl = 0; bl < values.size(); ++bl) {
        const size_t offset = 2 + (bl * BASELINES_BLOCK_LEN);
        double timestamp =
            time(buffer[offset], buffer[offset + 1]) - starttime;
        double baseline = ieee_float(buffer[offset + 2 + channel]);
        values[bl] = value(timestamp, baseline);
    }
}

param::value_type
baseline::compute_cut(size_t channel)
{
    param::value_type bl_cut = 0;
    double sdev = 0.0;
    size_t sdev_count = 0;

    for (size_t count = 0; count < 10; ++count) {
        get(channel);
        for (size_t bl = 0; bl < (values.size() - 1); ++bl) {
            double val =
                std::fabs(values[bl].second - values[bl + 1].second);
            if (val != 0) {
                if (val < (10.0 * values[bl].second) &&
                    val < (10.0 * values[bl + 1].second)) {
                    if (bl_cut == 0 || val < bl_cut) {
                        sdev += val;
                        ++sdev_count;
                    }
                }
            }
        }
    }

    if (sdev_count > 0) {
        constexpr double PI = double(3.14159265358979);
        constexpr double sqrpi = std::sqrt(PI / 2);
        double bl_sigma = sdev * sqrpi / sdev_count;
        bl_cut = static_cast<param::value_type>(std::floor(8.0 * bl_sigma));
    } else {
        bl_cut = 0;
    }

    return bl_cut;
}
}
}
}
