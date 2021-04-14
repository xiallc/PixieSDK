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

#include <hw/i2cm24c64.hpp>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace i2c
{
    i2cm24c64::i2cm24c64(module::module& module,
                         int reg,
                         uint32_t SDA,
                         uint32_t SCL,
                         uint32_t CTRL)
        : bitbash(module, reg, SDA, SCL, CTRL)
    {
    }

    void
    i2cm24c64::read(int address, size_t length, contents& data)
    {
        data.clear();
        data.reserve(length);

        /*
         *  Send "START"
         */
        start();

        /*
         * Send START, device select code, the address
         */
        write_ack(0xA0,
                  "i2cm24c64::sequential_read: no ACK after DevSel");
        write_ack(address >> 8,
                  "i2cm24c64::sequential_read: no ACK after addr (MSB)");
        write_ack(address & 0xff,
                  "i2cm24c64::sequential_read: no ACK after addr (LSB)");

        /*
         * Send "START", device select code, read data
         */
        start();
        write_ack(0xA1,
                  "i2cm24c64::sequential_read: no ACK after DevSel");
        for (size_t k = 0; k < length - 1; k++) {
            data.push_back(read_ack());
        }
        data.push_back(read_ack(false));

        /*
         * Send "STOP"
         */
        stop();
    }

    void
    i2cm24c64::read(contents& data)
    {
        read(0, size, data);
    }
};
};
};
};
