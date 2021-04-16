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

#include <iostream>
#include <iomanip>

#include <pixie_module.hpp>
#include <pixie_log.hpp>

#include <hw/csr.hpp>
#include <hw/memory.hpp>

#include <pixie16sys_defs.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace memory
{
    const address MCA_MEM_DATA = 0x00400000;

    mca::mca(module::module& module_)
        : module(module_)
    {
    }

    void
    mca::read(const address addr, words& values)
    {
        /*
         * Guard the PCI active  bit so it is cleared when we exit.
         */
        csr::set_clear csr(module, 1 << PCIACTIVE);

        /*
         * Set up the address to read from.
         */
        bus_write(WRT_EXT_MEM, addr);

        /*
         * Dummy read to make the FPGA not glitch at the end of addr write.
         */
        if (module >= module::module::rev_H) {
            (void) bus_read(MCA_MEM_DATA);
        }

        /*
         * Set up short FIFO in System FPGA
         */
        bus_write(SET_EXMEM_FIFO, 0);

        /*
         * Read the data using DMA.
         */
        module.dma_read(MCA_MEM_DATA, values);
    }

    void
    mca::write(const address addr, const words& values)
    {
        /*
         * Guard the PCI active  bit so it is cleared when we exit.
         */
        csr::set_clear csr(module, 1 << PCIACTIVE);

        bus_write(WRT_EXT_MEM, addr);
        for (auto value : values) {
            bus_write(MCA_MEM_DATA, value);
        }
    }
};
};
};
};
