#ifndef PIXIE_HW_H
#define PIXIE_HW_H

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

#include <array>
#include <stdexcept>
#include <vector>

#include <stdint.h>

#include <pixie_error.hpp>

namespace xia
{
namespace pixie
{
namespace hw
{
    /*
     * Revision tags
     */
    enum rev_tag {
        rev_A = 10,
        rev_B,
        rev_C,
        rev_D,
        rev_E,
        rev_F,
        rev_G,
        rev_H,
        rev_I,
        rev_J,
        rev_K,
        rev_L
    };

    /*
     * Maximum number of slots in a crate.
     */
    static const int max_slots = 13;

    /*
     * Maximum number of channels a module can have.
     */
    static const int max_channels = 32;

    /*
     * Maximum ADC trace size.
     */
    static const size_t max_adc_trace_length = 8192;

    /*
     * Maximum histogram size.
     */
    static const size_t max_histogram_length = 32768;

    /*
     * IO buffer length
     */
    static const size_t io_buffer_length = 65536;

    /*
     * System FPGA clock frequency in MHz
     */
    static const size_t system_clock_mhz = 100;

    /*
     * FIFO size in words
     */
    static const size_t fifo_size_words = 32768;

    /*
     * DMA block size.
     */
    static const size_t max_dma_block_size = 8192;

    /*
     * EEPROM block size
     */
    static const size_t eeprom_block_size = 128;

    /*
     * Address.
     */
    typedef uint32_t address;

    /*
     * The read/write word.
     */
    typedef uint32_t word;

    /*
     * Word pointer.
     */
    typedef word* word_ptr;

    /*
     * Words
     */
    typedef std::vector<word> words;

    /*
     * ADC trace word
     */
    typedef uint16_t adc_word;

    /*
     * IO buffer
     */
    typedef std::array<word, io_buffer_length> io_buffer;

    /*
     * ADC trace buffer, as read from the channel
     */
    typedef std::array<word, max_adc_trace_length> adc_trace_buffer;

    /*
     * ADC trace
     */
    typedef std::vector<adc_word> adc_trace;

    /*
     * Hardware errors
     */
    typedef error::error error;

    /*
     * Configuration
     */
    struct config {
        /*
         * Channel index in variable array.
         */
        int index;

        /*
         * Analog front end (AFE)
         */
        int adc_bits;
        int adc_msps;
        int adc_clk_div;
        int fpga_clk_mhz;

        config(int adc_bits,
               int adc_msps,
               int adc_clk_div,
               int fpga_clk_mhz);
        config();

        bool operator==(const config& cfg);
        bool operator!=(const config& cfg);

        void clear();
    };

    typedef std::vector<config> configs;

    /*
     * Convertor. Use with caution as this steps around the type system.
     */
    template <typename I, typename O>
    inline void convert(I vin, O& vout) {
        vout = static_cast<O>(vin);
    }

    /*
     * Wait in microseconds. We need to check how well this works.
     */
    void wait(size_t microseconds);

    /*
     * Bus interface calls.
     */
    static inline word
    read_word(void* addr, const int offset) {
        volatile word* p = static_cast<volatile word*>(addr);
        return *(p + (offset / 4));
    }

    static inline void
    write_word(void* addr, const int offset, const word value) {
        volatile word* p = static_cast<volatile word*>(addr);
        *(p + (offset / 4)) = value;
    }
}
}
}

#endif  // PIXIE_HW_H
