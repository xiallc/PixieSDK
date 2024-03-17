/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file db04.cpp
 * @brief Implements DB04
 */

#include <algorithm>
#include <cstring>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/db04.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

db04::db04(
    pixie::module::module& module_, const hw::db_assembly& db_assembly)
    : db(module_, db_assembly) {
}

db04::~db04() {
}

void db04::init_channels() {
    init_db_channels<db04_channel>(*this);
}

db04_channel::db04_channel(
    pixie::channel::channel& mod_chan, db& board, const hw::config& config_)
    : channel(mod_chan, board, config_) {
}

db04_channel::~db04_channel() {
}

void db04_channel::set_dac(param::value_type value) {
    pixie::module::module& mod = get_module();
    if (value >= (1 << 16)) {
        throw error::error(
            error::code::invalid_value,
            pixie::module::module_label(mod, "DB04") + "invalid DAC offset: channel=" +
            std::to_string(board.base.get<size_t>() + offset.get<size_t>()));
    }
    int num = board.number.get<int>();
    int off = offset.get<int>();
    /*
     * Select the module port
     */
    mod.select_port(num + 1);
    /*
     * Address bit 1 selects DAC for the upper 4 channels. Clear bit 0 and set
     * bit 1 if the DB channel offset is less than 4.
     */
    hw::word dac_addr = 0x20 | ((off < 4 ? 1 : 0) << 1);
    /*
     * Compensate for PCB ADC swapping:
     *
     * Channel offset DAC Output
     *     0, 4           B (1)
     *     1, 5           C (2)
     *     2, 6           A (0)
     *     3, 7           D (3)
     */
    hw::word dac_ctrl = 0x30;
    switch (off) {
    case 0:
    case 4:
        dac_ctrl += 1;
        break;
    case 1:
    case 5:
        dac_ctrl += 2;
        break;
    case 2:
    case 6:
        dac_ctrl += 0;
        break;
    case 3:
    case 7:
        dac_ctrl += 3;
        break;
    default:
        break;
    }
    /*
     * CFG_DAC expacts [addr(8), ctrl(8), data(16)]
     */
    const hw::word dac = (dac_addr << 24) | (dac_ctrl << 16) | value;
    log(log::debug) << pixie::module::module_label(mod, "fixture: db04")
                    << "db=" << num
                    << " db_channel=" << off
                    << std::hex
                    << " dac_addr=0x" << dac_addr
                    << " dac_ctrl=0x" << dac_ctrl
                    << " dac_value=0x" << value
                    << " write=0x" << dac;
    mod.write_word(hw::device::CFG_DAC, dac);
     /*
     * It takes about 4ms to clock out the 32 bits
     */
    hw::wait(6000);
}

void db04_channel::get(const std::string item, bool& value) {
    if (item == "dac.has.offset") {
        value = true;
    } else {
        channel::get(item, value);
    }
}

void db04_channel::get(const std::string item, unsigned int& value) {
    if (item == "dac.settle-period") {
        value = db04::dac_settle_time_ms;
    } else {
        channel::get(item, value);
    }
}

}   // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
