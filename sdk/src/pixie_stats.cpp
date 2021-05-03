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
#include <pixie_stats.hpp>

#include <hw/memory.hpp>

namespace xia
{
namespace pixie
{
namespace stats
{
static uint64_t
make_u64(const param::value_type high, const param::value_type low)
{
    return (uint64_t(high) << 32) | uint64_t(low);
}

channel::channel(const hw::config& config)
    : fast_peaks_a(0),
      fast_peaks_b(0),
      live_time_a(0),
      live_time_b(0),
      chan_events_a(0),
      chan_events_b(0),
      runtime_a(0),
      runtime_b(0),
      config(config)
{
}

channel::channel()
    : fast_peaks_a(0),
      fast_peaks_b(0),
      live_time_a(0),
      live_time_b(0),
      chan_events_a(0),
      chan_events_b(0),
      runtime_a(0),
      runtime_b(0)
{
}

double
channel::input_count_rate() const
{
    const double fast_peaks = make_u64(fast_peaks_a, fast_peaks_b);
    const double live_time =  make_u64(live_time_a, live_time_b);
    if (live_time == 0.0) {
        return 0.0;
    }
    return fast_peaks / (live_time * (1.0e-6 / config.adc_msps));
}

double
channel::output_count_rate() const
{
    const double chan_events = make_u64(chan_events_a, chan_events_b);
    const double real_time =  make_u64(runtime_a, runtime_b);
    if (real_time == 0.0) {
        return 0.0;
    }
    return chan_events / (real_time * (1.0e-6 / config.adc_msps));
}

module::module()
    : num_events_a(0),
      num_events_b(0)
{
}

uint64_t
module::processed_events() const
{
    return make_u64(num_events_a, num_events_b);
}

stats::stats(const hw::configs& configs)
{
    chans.resize(configs.size());
    for (size_t channel = 0; channel < configs.size(); ++channel) {
        chans[channel].config = configs[channel];
    }
}

void
read(pixie::module::module& module_, stats& stats_)
{
    const hw::address base = module_.param_addresses.module_out.start;
    const size_t words =
        module_.param_addresses.module_out.size +
        module_.param_addresses.channels_out.size;

    hw::memory::dsp dsp(module_);
    hw::words vars(words);

    dsp.read(base, vars);

    const param::module_var_descs& mod_descs = module_.module_var_descriptors;
    const param::channel_var_descs& chan_descs = module_.channel_var_descriptors;

    hw::address addr;

    addr = param::get_descriptor(mod_descs,
                                 param::module_var::NumEventsA).address;
    stats_.mod.num_events_a = vars[addr - base];
    addr = param::get_descriptor(mod_descs,
                                 param::module_var::NumEventsB).address;
    stats_.mod.num_events_b = vars[addr - base];

    addr = param::get_descriptor(mod_descs,
                                 param::module_var::RunTimeA).address;
    param::value_type runtime_a = vars[addr - base];
    addr = param::get_descriptor(mod_descs,
                                 param::module_var::RunTimeB).address;
    param::value_type runtime_b = vars[addr - base];

    for (auto& channel : stats_.chans) {
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::FastPeaksA).address;
        channel.fast_peaks_a = vars[addr - base];
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::FastPeaksB).address;
        channel.fast_peaks_b = vars[addr - base];
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::LiveTimeA).address;
        channel.live_time_a = vars[addr - base];
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::LiveTimeB).address;
        channel.live_time_b = vars[addr - base];
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::ChanEventsA).address;
        channel.chan_events_a = vars[addr - base];
        addr = param::get_descriptor(chan_descs,
                                     param::channel_var::ChanEventsB).address;
        channel.chan_events_b = vars[addr - base];
        channel.runtime_a = runtime_a;
        channel.runtime_b = runtime_b;
    }
}
}
}
}
