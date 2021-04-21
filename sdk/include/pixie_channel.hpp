#ifndef PIXIE_CHANNEL_H
#define PIXIE_CHANNEL_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
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

#include <pixie_error.hpp>
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
namespace channel
{
/*
 * FIFO
 */
struct fifo {
    module::module& module;

    fifo(module::module& module);

    void update(const size_t channel, param::value_type trace_delay);
};

/*
 * Baseline
 */
struct baseline {
    static const size_t max_num = 3640;

    typedef std::pair<double, double> value;

    module::module& module;
    std::vector<value> values;

    baseline(module::module& module, size_t num = max_num);

    void find_cut(size_t channel);

    double time(hw::word time_word0, hw::word time_word1);

    void get(size_t channel);

private:

    param::value_type compute_cut(size_t channel);
};
}
}
}


#endif  // PIXIE_CHANNEL_H
