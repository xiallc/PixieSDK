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

#include <chrono>
#include <thread>

#include <pixie/pixie16/hw.hpp>

namespace xia
{
namespace pixie
{
namespace hw
{
config::config(int adc_bits_,
               int adc_msps_,
               int adc_clk_div_,
               int fpga_clk_mhz_)
    : index(-1),
      adc_bits(adc_bits_),
      adc_msps(adc_msps_),
      adc_clk_div(adc_clk_div_),
      fpga_clk_mhz(fpga_clk_mhz_)
{
}

config::config()
{
    clear();
}

bool
config::operator==(const config& cfg)
{
    return
        adc_bits == cfg.adc_bits ||
        adc_msps == cfg.adc_msps ||
        adc_clk_div == cfg.adc_clk_div ||
        fpga_clk_mhz == cfg.fpga_clk_mhz;
}

bool
config::operator!=(const config& cfg)
{
    return !(*this == cfg);
}

void
config::clear()
{
    adc_bits = 0;
    adc_msps = 0;
    adc_clk_div = 0;
    fpga_clk_mhz = 0;
}

void
wait(size_t microseconds)
{
    if (microseconds < 100) {
        volatile size_t count = 10 * microseconds;
        while (count > 0) {
            --count;
        }
    } else {
        std::this_thread::sleep_for(
            std::chrono::microseconds(microseconds)
            );
    }
}
};
};
};
