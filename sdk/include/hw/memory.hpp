#ifndef PIXIE_HW_MEMORY_H
#define PIXIE_HW_MEMORY_H

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

#include <stdint.h>

#include <pixie_fw.hpp>
#include <pixie_hw.hpp>
#include <pixie_fw.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
    class module;
}
namespace hw
{
namespace memory
{
    /*
     * Bus addresses
     */
    static const address IO_BUFFER_ADDR = 0x00050000; /* R */
    static const address FIFO_MEM_DMA   = 0x00200000; /* R */
    static const address DSP_MEM_DMA    = 0x00300000; /* R */
    static const address MCA_MEM_DMA    = 0x00400000; /* R */
    static const address MCA_MEM_DATA   = 0x00400000; /* R W */

    /*
     * MCA addresses
     */
    static const address HISTOGRAM_MEMORY = 0x00000000;

    struct bus
    {
        module::module& module;

        bus(module::module& module);

    protected:
        /*
         * Low level access.
         */
        void bus_write(int reg, uint32_t data) {
            module.write_word(reg, data);
        }
        uint32_t bus_read(int reg) {
            return module.read_word(reg);
        }
    };

    struct dsp
        : public bus
    {
        /*
         * DMA block size.
         */
        static const size_t max_dma_block_size = 8192;

        dsp(module::module& module);

        /*
         * Memory read.
         */
        word read(const address addr);
        word read(const size_t channel, const address addr);

        /*
         * Memory block read.
         *
         * If length is not 0 only the length of data will be read.
         */
        template<class B>
        void read(const address addr, B& values, const size_t length = 0);
        void read(const address addr, word_ptr buffer, const size_t length);

        /*
         * Memory write.
         */
        void write(const address addr, const word value);
        void write(const size_t channel,
                   const address addr, const word value);

        /*
         * Memory block write.
         */
        void write(const address addr, const words& values);
        void write(const size_t channel,
                   const address addr, const words& values);

    private:

        /*
         * DMA set up
         */
        void dma_read(const address addr,
                      word_ptr buffer,
                      const size_t length);
    };

    template<class B>
    inline void dsp::read(const address addr, B& values, const size_t length) {
        if (length > values.size()) {
            throw error(error::code::invalid_value,
                        "dsp: read length greater than buffer size");
        }
        read(addr, values.data(), length != 0 ? length : values.size());
    }

    struct mca
        : public bus
    {
        mca(module::module& module);

        /*
         * Block read and write.
         */
        void read(const address addr, words& values);
        void read(const address addr, word_ptr values, size_t size);
        void write(const address addr, const words& values);
    };
}
}
}
}

#endif  // PIXIE_HW_MEMORY_H
