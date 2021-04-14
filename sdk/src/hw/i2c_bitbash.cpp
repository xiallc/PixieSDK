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

#include <hw/i2c_bitbash.hpp>

namespace xia
{
namespace pixie
{
namespace hw
{
namespace i2c
{
    bitbash::bitbash(module::module& module_,
                     int reg_,
                     uint32_t SDA_,
                     uint32_t SCL_,
                     uint32_t CTRL_,
                     bool trace_)
        : module(module_),
          reg(reg_),
          SDA(SDA_),
          SCL(SCL_),
          CTRL(CTRL_),
          trace(trace_)
    {
    }

    bitbash::~bitbash()
    {
        if (module.present())
            stop();
    }

    void
    bitbash::start()
    {
        if (trace) {
            log(log::debug) << "i2c-bb: start";
        }

        /*
         * Set SCL and SDA to 1, then set SDA to 0 while keeping SCL at 1
         *
         * The CTRL is a tristate enable control bit for SDA
         */
        bus_write(SDA | SCL | CTRL);
        bus_write(SCL | CTRL);
    }

    void
    bitbash::stop()
    {
        uint32_t pins = bus_read() & (SDA | SCL | CTRL);

        if (trace) {
            log(log::debug) << "i2c-bb: stop " << std::hex << pins;
        }

        /*
         * Set SCL and SDA to 1, then set SDA to 0 while keep SCL at 1
         */
        if (pins != (SDA | SCL | CTRL)) {
            bus_write(SCL | CTRL);
            bus_write(SDA | SCL | CTRL);
        }
    }

    void
    bitbash::write_ack(uint8_t data, const char* what)
    {
        if (trace) {
            log(log::debug) << "i2c-bb: write_ack";
        }

        write(data);
        if (!get_ack()) {
            stop();
            throw error(error::code::device_hw_failure, what);
        }
    }

    uint8_t
    bitbash::read_ack(bool ack)
    {
        if (trace) {
            log(log::debug) << "i2c-bb: read_ack " << ack;
        }

        uint8_t data = read();
        if (ack) {
            send_ack();
        } else {
            send_nack();
        }
        return data;
    }

    void
    bitbash::write(uint8_t data)
    {
        if (trace) {
            log(log::info) << "i2c-bb: write " << std::hex << (int) data;
        }

        uint32_t data_bit = 0;

        for (int bit = 7; bit >= 0; bit--) {
            /*
             * SDA = 0; SCL = 0; CTRL = 1
             */
            bus_write(CTRL | data_bit);

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
    bitbash::read()
    {
        if (trace) {
            log(log::debug) << "i2c-bb: read";
        }

        uint32_t data_bit = 0;
        uint8_t data = 0;

        for (int bit = 7; bit >= 0; bit--) {
            /*
             * SDA = 0; SCL = 1; CTRL = 0
             */
            bus_write(SCL);

            /*
             * Read the bit, MSB to LSB.
             */
            data_bit = bus_read() & SDA;
            if (data_bit != 0) {
                data |= 1 << bit;
            }

            /*
             * SDA = data_bit; SCL = 0; CTRL = 0
             */
            bus_write(data_bit);
        }

        if (trace) {
            log(log::debug) << "i2c-bb: read " << std::hex << (int) data;
        }

        return data;
    }

    bool
    bitbash::get_ack()
    {
        if (trace) {
            log(log::debug) << "i2c-bb: get_ack";
        }

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

        if (trace) {
            log(log::debug) << "i2c-bb: get_ack " << ((data & 0x1) == 0);
        }

        return (data & 0x1) == 0;
    }

    void
    bitbash::send_ack()
    {
        if (trace) {
            log(log::debug) << "i2c-bb: send_ack";
        }

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
    bitbash::send_nack()
    {
        if (trace) {
            log(log::debug) << "i2c-bb: send_nack";
        }

        /*
         * SDA = 1; SCL = 0; CTRL = 1
         */
        bus_write(CTRL);
        bus_write(SDA | CTRL);

        /*
         * SDA = 0; SCL = 1; CTRL = 1
         */
        bus_write(SDA | SCL | CTRL);

        /*
         * SDA = 0; SCL = 0; CTRL = 0
         */
        bus_write(0);
    }

    void
    bitbash::bus_write(uint8_t data)
    {
        module.write_32(reg, data);
        wait(5);
    }

    uint8_t
    bitbash::bus_read()
    {
        return module.read_32(reg);
    }
};
};
};
};
