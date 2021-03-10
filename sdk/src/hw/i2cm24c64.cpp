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
    i2cm24c64::i2cm24c64(module::module& module_,
                         int reg_,
                         uint32_t SDA_,
                         uint32_t SCL_,
                         uint32_t CTRL_)
        : module(module_),
          reg(reg_),
          SDA(SDA_),
          SCL(SCL_),
          CTRL(CTRL_)
    {
    }

    void
    i2cm24c64::sequential_read(int address,
                               size_t length,
                               std::vector<uint8_t> data)
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
            data.push_back(read_ack(true));
        }
        data.push_back(read_ack(false));

        /*
         * Send "STOP"
         */
        stop();
    }

    void
    i2cm24c64::start()
    {
        /*
         * Set SCL and SDA to 1, then set SDA to 0 while keep SCL at 1
         */
        bus_write(SDA | SCL | CTRL);
        bus_write(SCL | CTRL);
    }

    void
    i2cm24c64::stop()
    {
        /*
         * Set SCL and SDA to 1, then set SDA to 0 while keep SCL at 1
         */
        bus_write(SDA | SCL | CTRL);
        bus_write(SCL | CTRL);
    }

    void
    i2cm24c64::write_ack(uint8_t data, const char* what)
    {
        write(data);
        if (!get_ack()) {
            throw error(what);
        }
    }

    uint8_t
    i2cm24c64::read_ack(bool ack)
    {
        uint8_t data = read();
        if (ack) {
            send_ack();
        }
        return data;
    }

    void
    i2cm24c64::write(uint8_t data)
    {
        uint32_t data_bit = 0;

        for (ssize_t bit = 7; bit >= 0; bit--) {
            /*
             * SDA = 0; SCL = 0; CTRL = 1
             */
            bus_write(CTRL);

            /*
             * Get the bit to send, MSB to LSB.
             */
            if ((data & (1 << bit)) != 0) {
                data_bit = SDA;
            } else {
                data_bit = 0;
            }

            /*
             * SDA = data_bit; SCL = 0; CTRL = 1
             */
            bus_write(CTRL | data_bit);

            /*
             * SDA = data_bit; SCL = 1; CTRL = 1
             */
            bus_write(CTRL | SCL | data_bit);
        }

        /*
         * SDA = data_bit; SCL = 0; CTRL = 0
         */
        bus_write(data_bit);
    }

    uint8_t
    i2cm24c64::read()
    {
        uint32_t data_bit = 0;
        uint8_t data = 0;

        for (ssize_t bit = 7; bit >= 0; bit--) {
            /*
             * SDA = 0; SCL = 1; CTRL = 0
             */
            bus_write(SCL);

            /*
             * Get the bit to send, MSB to LSB.
             */
            data_bit = bus_read();
            if ((data_bit & SDA) != 0) {
                data |= 1 << bit;
            }

            /*
             * SDA = data_bit; SCL = 0; CTRL = 0
             */
            bus_write(data_bit & SDA);

            /*
             * SDA = data_bit; SCL = 1; CTRL = 1
             */
            bus_write(CTRL | SCL | data_bit);
        }

        return data;
    }

    bool
    i2cm24c64::get_ack()
    {
        uint32_t data;

        /*
         * SDA = 0; SCL = 1; CTRL = 0
         */
        bus_write(SCL);

        /*
         * Read SDA
         */
        data = bus_read();

       /*
        * SDA = 0; SCL = 0; CTRL = 0
        */
        bus_write(0);

        return (data & 0x1) == 0;
    }

    void
    i2cm24c64::send_ack()
    {
        /*
         * SDA = 0; SCL = 0; CTRL = 1
         */
        bus_write(CTRL);

        /*
         * SDA = 0; SCL = 1; CTRL = 1
         */
        bus_write(SCL | CTRL);

        /*
         * SDA = 0; SCL = 0; CTRL = 0
         */
        bus_write(0);
    }

    void
    i2cm24c64::bus_write(uint8_t data)
    {
        module.write_32(reg, data);
        wait(10);
    }

    uint8_t
    i2cm24c64::bus_read()
    {
        return module.read_32(reg);
    }
};
};
};
};
