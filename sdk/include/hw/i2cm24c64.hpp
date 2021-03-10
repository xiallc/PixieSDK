#ifndef PIXIE_HW_I2CM24C64_H
#define PIXIE_HW_I2CM24C64_H

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

#include <stdint.h>

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
namespace i2c
{
    struct i2cm24c64
    {
        module::module& module;

        /*
         * The register offset in the module's address space.
         */
        const int reg;

        /*
         * Bit mask of the signals.
         */
        const uint32_t SDA;
        const uint32_t SCL;
        const uint32_t CTRL;

        i2cm24c64(module::module& module,
                  int reg,
                  uint32_t SDA,
                  uint32_t SCL,
                  uint32_t CTRL);

        /*
         * Read the EEPROM.
         */
        void sequential_read(int address,
                             size_t length,
                             std::vector<uint8_t> data);

        /*
         * Bus control
         */
        void start();
        void stop();

        /*
         * Byte level writes and reads with ACKs
         */
        void write_ack(uint8_t data, const char* what);
        uint8_t read_ack(bool ack);

        /*
         * Low level byte wide access.
         */
        void write(uint8_t data);
        uint8_t read();

        /*
         * ACK control
         */
        bool get_ack();
        void send_ack();

        /*
         * Low level I2C access.
         */
        void bus_write(uint8_t data);
        uint8_t bus_read();
    };
}
}
}
}

#endif  // PIXIE_HW_I2CM24C64_H
