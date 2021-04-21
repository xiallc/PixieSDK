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
#include <def21160.h>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace memory
{
    struct host_bus_request {
        module::module& module;
        bool holding;
        host_bus_request(module::module& module_)
            : module(module_),
              holding(false) {
            request();
        }
        ~host_bus_request() {
            release();
        }
        void request() {
            if (!holding) {
                module.write_word(REQUEST_HBR, 0);
                holding = true;
            }
        }
        void release() {
            if (holding) {
                module.write_word(HBR_DONE, 0);
                holding = false;
            }
        }
    };

    bus::bus(module::module& module_)
        : module(module_)
    {
    }

    dsp::dsp(module::module& module_)
        : bus(module_)
    {
    }

    word
    dsp::read(const address addr)
    {
        host_bus_request hbr(module);
        bus_write(EXT_MEM_TEST, addr);
        word value = bus_read(WRT_DSP_MMA);
        return value;
    }

    word
    dsp::read(const size_t channel, const address addr)
    {
        return read(addr + (channel * sizeof(word)));
    }

    void
    dsp::read(const address addr, words& values)
    {
        size_t size = values.size();
        word_ptr block_data = values.data();
        address block_addr = addr;
        while (size > 48) {
            const size_t block_size =
                size > max_dma_block_size ? max_dma_block_size : size;
            dma_read(block_addr, block_data, block_size);
            size -= block_size;
        }
        if (size > 0) {
            host_bus_request hbr(module);
            bus_write(EXT_MEM_TEST, block_addr);
            while (size-- > 0) {
                *block_data = bus_read(WRT_DSP_MMA);
                block_data++;
            }
       }
    }

    void
    dsp::read(const address addr, io_buffer& buffer)
    {
        static_assert((io_buffer_length % max_dma_block_size) != 0,
                      "io-buffer not a multiple of the block size");
        const size_t blocks = buffer.size() / max_dma_block_size;
        word* data = buffer.data();
        size_t offset = 0;
        for (size_t block = 0; block < blocks; ++block) {
            dma_read(addr + offset,  data + offset, max_dma_block_size);
            offset += max_dma_block_size;
        }
    }

    void
    dsp::write(const address addr, const word value)
    {
        host_bus_request hbr(module);
        bus_write(EXT_MEM_TEST, addr);
        bus_write(WRT_DSP_MMA, value);
    }

    void
    dsp::write(const size_t channel, const address addr, const word value)
    {
        write(addr + (channel * sizeof(word)), value);
    }

    void
    dsp::write(const address addr, const words& values)
    {
        host_bus_request hbr(module);
        bus_write(EXT_MEM_TEST, addr);
        for (auto value : values) {
            bus_write(WRT_DSP_MMA, value);
        }
    }

    void
    dsp::dma_read(const address addr, word_ptr buffer, const size_t length)
    {
        host_bus_request hbr(module);

        bus_write(EXT_MEM_TEST, DMASTAT);
        if ((bus_read(WRT_DSP_MMA) & (1 << 11)) != 0) {
            throw error(error::code::device_dma_busy, "dsp: DMA busy");
        }

        bus_write(WRT_DSP_II11, addr);
        bus_write(WRT_DSP_C11, length);
        bus_write(WRT_DSP_IM11, 1);
        bus_write(WRT_DSP_EC11, length);
        bus_write(WRT_DSP_DMAC11, 0x905);
        bus_write(RD_WRT_FIFO_WML, length / 2);

        hbr.release();

        try {
            bus_write(SET_INT_FIFO, 0);
            csr::fifo_ready_wait(module);
            module.dma_read(addr, buffer, length);
        } catch (...) {
            hbr.request();
            bus_write(WRT_DSP_DMAC11, 0x904);
            throw;
        }

        hbr.request();
        bus_write(WRT_DSP_DMAC11, 0x904);
    }

    void
    dsp::write(const size_t channel,
               const address addr, const words& values)
    {
        write(addr + (channel * sizeof(word)), values);
    }

    mca::mca(module::module& module_)
        : bus(module_)
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
