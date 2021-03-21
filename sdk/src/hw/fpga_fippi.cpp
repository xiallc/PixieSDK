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

#include <hw/fpga_fippi.hpp>

#include <pixie16sys_defs.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace fpga
{
    fippi::fippi(module::module& module_, bool trace)
        : module(module_),
          ctrl_1_2(module,
                   "fippi-1-2",
                   control::controls(0xfffff000, 0x000000f2, 0x0a3),
                   control::controls(0xfffff000, 0x00000052, 0x053),
                   control::regs(CFG_DATACS, CFG_CTRLCS, CFG_RDCS),
                   trace),
          ctrl_3_4(module,
                   "fippi-3-4",
                   control::controls(0xfffff000, 0x00000fa2, 0xaa3),
                   control::controls(0xfffff000, 0x000005a2, 0x5a3),
                   control::regs(CFG_DATACS, CFG_CTRLCS, CFG_RDCS),
                   trace)
    {
    }

    void
    fippi::boot(const firmware::image& image, int retries)
    {
        ctrl_1_2.load(image, retries);
        ctrl_3_4.load(image, retries);
        wait(10000);
        module.write_32(CFG_DCMRST, 0);
    }
};
};
};
};
