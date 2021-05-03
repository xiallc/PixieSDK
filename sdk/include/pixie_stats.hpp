#ifndef PIXIE_STATS_H
#define PIXIE_STATS_H

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

#include <vector>

#include <pixie_hw.hpp>
#include <pixie_param.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
    class module;
}
namespace stats
{
/*
 * Statistics
 *
 * The channel and module stats cannot contain any containers while they
 * are used in the legacy API.
 */
struct channel {
    param::value_type fast_peaks_a;
    param::value_type fast_peaks_b;
    param::value_type live_time_a;
    param::value_type live_time_b;
    param::value_type chan_events_a;
    param::value_type chan_events_b;
    param::value_type runtime_a;
    param::value_type runtime_b;

    hw::config config;

    channel(const hw::config& config);
    channel();

    double input_count_rate() const;
    double output_count_rate() const;
};

typedef std::vector<channel> channels;

struct module {
    param::value_type num_events_a;
    param::value_type num_events_b;

    module();

    uint64_t processed_events() const;
};

    typedef std::vector<module> modules;

struct stats {
    module mod;
    channels chans;

    stats(const hw::configs& configs);
};

void read(pixie::module::module& module_, stats& stats_);
}
}
}

#endif  // PIXIE_STATS_H
