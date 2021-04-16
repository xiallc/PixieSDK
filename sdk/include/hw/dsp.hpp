#ifndef PIXIE_HW_DSP_H
#define PIXIE_HW_DSP_H

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
namespace dsp
{
    struct dsp
    {
        /*
         * The word loaded into the DSP.
         */
        typedef uint16_t load_value_type;

        module::module& module;

        /*
         * Online?
         */
        bool online;

        /*
         * Trace the load operation
         */
        bool trace;

        dsp(module::module& module, bool trace = false);
        dsp& operator=(dsp&& d);

        /*
         * Boot
         */
        void boot(const firmware::image& image, int retries = 10);

        /*
         * Is the DSP loaded and runings?
         */
        bool done();

        /*
         * Memory read.
         */
        word read(const address addr);
        word read(const size_t channel, const address addr);

        /*
         * Memory write.
         */
        void write(const address addr, const word value);
        void write(const size_t channel,
                   const address addr, const word value);

        /*
         * Memory write.
         */
        void write(const address addr, const words& values);
        void write(const size_t channel,
                   const address addr, const words& values);

    private:

        /*
         * Checked write, writes and value then checks for a reply.
         */
        bool checked_write(const word out,
                           const word value,
                           const word in,
                           const word result,
                           const int out_wait = 0,
                           const int in_wait = 1000);

        /*
         * Image section loader.
         */
        void section_load(firmware::reader& reader, const size_t wordsize);

        /*
         * Low level access.
         */
        void bus_write(int reg, uint32_t data);
        uint32_t bus_read(int reg);

        std::string make_what(const char* msg);
    };
}
}
}
}

#endif  // PIXIE_HW_DSP_H
