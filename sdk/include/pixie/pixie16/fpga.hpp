#ifndef PIXIE_HW_FPGA_H
#define PIXIE_HW_FPGA_H

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

#include <cstdint>

#include <pixie/fw.hpp>

#include <pixie/pixie16/hw.hpp>

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
namespace fpga
{
    struct control
    {
        /*
         * Controls.
         */
        struct controls {
            const uint32_t clear;
            const uint32_t set;
            const uint32_t done;

            controls(const uint32_t clear,
                     const uint32_t set,
                     const uint32_t done);
        };

        /*
         * Registers.
         */
        struct regs {
            const int DATACS;
            const int CTRLCS;
            const int RDCS;

            regs(const int DATACS,
                 const int CTRLCS,
                 const int RDCS);
        };

        module::module& module;
        const std::string name;
        const controls load_ctrl;
        const controls clear_ctrl;
        const regs reg;

        /*
         * Trace the load operation
         */
        const bool trace;

        control(module::module& module,
                const std::string name,
                const controls& load_ctrl,
                const controls& clear_ctrl,
                const regs& reg,
                bool trace = false);

        /*
         * Load
         */
        void load(const firmware::image& image, int retries = 10);

        /*
         * Has the programming completed?
         */
        bool done();

        /*
         * Low level access.
         */
        void bus_write(int reg, uint32_t data);
        uint32_t bus_read(int reg);

    private:

        std::string make_what(const char* msg);
    };
}
}
}
}

#endif  // PIXIE_HW_FPGA_H
