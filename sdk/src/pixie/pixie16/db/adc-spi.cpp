/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file adc-spi.cpp
 * @brief Implements ADC SPI driver
 */

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/adc-spi.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

static hw::word adc_value(
    param::value_type chan, bool read_not_write, param::value_type reg, param::value_type value) {
    hw::word val =
        ((1 << chan) << 24) | ((reg & 0xff) << 8) | (value & 0xff);
    if (read_not_write) {
        val |= 1 << 23;
    }
    return val;
}

adc_spi_driver::adc_spi_driver(db& board_)
    : board(board_) {
    log(log::info) << db_label(board) << "init adc spi driver";
}

adc_spi_driver::~adc_spi_driver() {
}

void adc_spi_driver::write_reg(channel& chan, param::value_type reg, param::value_type value) {
    auto& mod = chan.module_channel.module.get();
    auto chan_num = chan.offset.get<param::value_type>();
    hw::memory::fippi fippi(mod);
    hw::address addr = hw::fippi_addr(board.number.get<hw::address>(), hw::fippi::ADCSPI);
    hw::word val = adc_value(chan_num, false, reg, value);
    log(log::debug) << db_label(board)
                    << chan_num <<  std::hex
                    << ": adc: write: reg=0x" << reg << " value=0x" << value
                    << " (addr=0x" << addr << " write=0x" << val << ')';
    fippi.write(addr, val);
     /*
     * It takes about 4ms to clock out the 32 bits.
     */
    hw::wait(4000);
}

param::value_type adc_spi_driver::read_reg(channel& chan, param::value_type reg) {
    log(log::warning) << db_label(board)
                      << "adc spi: reads are not implemented in the firmware";
    auto& mod = chan.module_channel.module.get();
    auto chan_num = chan.offset.get<param::value_type>();
    hw::memory::fippi fippi(mod);
    hw::address addr = hw::fippi_addr(board.number.get<hw::address>(), hw::fippi::ADCSPI);
    hw::word wval = adc_value(chan_num, true, reg, 0);
    fippi.write(addr, wval);
    hw::wait(4000);
    hw::address raddr = hw::fippi_addr(board.number.get<hw::address>(), hw::fippi::ADCFRAME);
    hw::word val = fippi.read(raddr);
    log(log::debug) << db_label(board)
                    << chan_num << std::hex
                    << ": adc: read: reg=0x" << reg << " value=0x" << (val & 0xff)
                    << " (addr=0x" << raddr << " write=0x" << wval << " read=0x" << val << ')';
    return val & 0xff;
}
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
