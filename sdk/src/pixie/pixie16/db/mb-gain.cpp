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

/** @file mb-gain.cpp
 * @brief Implements Montherboard Gain control
 */

#include <limits>
#include <mutex>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/mb-gain.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

static constexpr uint16_t gain_mask[2][4] = {
    { 1 << 0,                 /* 0 */
      1 << 11,                /* 1 */
      1 << 2,                 /* 2 */
      1 << 5 },               /* 3 */
    { 1 << 8,                 /* 0 */
      1 << 10,                /* 1 */
      1 << 4,                 /* 2 */
      1 << 6 }                /* 3 */
};

mb_channel_gain::mb_channel_gain(channel& channel__, mb_gain_driver& driver_)
    : channel_(channel__), driver(driver_) {
    xia::log(log::debug) << channel_label(channel_) << "init channel gain";
    auto label = channel_.get_mib_base() + "gain" + mib::mibsep;
    level = mib::node(label + "level", mib::type::uinteger);
    mib::event_func step_setter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            self.set_gain(data.get<param::value_type>());
        };
    level.set_event_func(mib::event::set, step_setter);
    level = 0;
}

mb_channel_gain::~mb_channel_gain() {
    level.remove();
}

void mb_channel_gain::set_gain(param::value_type value) {
    auto& mod = driver.board.module_;
    if (value >= driver.steps.get<param::value_type>()) {
        throw error::error(
            error::code::invalid_value,
            pixie::module::module_label(mod, "gain") +
            ": invalid gain: " + std::to_string(value));
    }
    if (!level.valid()) {
        throw error::error(
            error::code::internal_failure,
            pixie::module::module_label(mod, "gain") +
            ": level not initialised");
    }
    driver.set_gain(*this, value);
    level = value;
}

void mb_channel_gain::enable_mibs() {
    level.enable();
}

void mb_channel_gain::disable_mibs() {
    level.disable();
}

mb_gain_driver::mb_gain_driver(db& board_) : board(board_) {
    log(log::debug) << db_label(board) << "init gain driver";
    auto mib_base = board.get_mib_base() + "gain" + mib::mibsep;
    mib::add_ro_int(mib_base + "steps", 2);
    steps = mib::find(mib_base + "steps");
    latch = mib::node(mib_base + "latch", mib::type::uinteger);
    latch = mib::hint::fmt_hex;
    latch = 0xffff;
    latch.lock_writes();
}

mb_gain_driver::~mb_gain_driver() {
    steps.remove();
    latch.remove();
}

void mb_gain_driver::set_gain(mb_channel_gain& channel_gain, param::value_type value) {
    lock_guard guard(lock);
    auto& mod = channel_gain.channel_.get_module();
    auto& board = channel_gain.channel_.board;
    size_t db_chan = channel_gain.channel_.offset;
    const uint16_t latch_cur = latch.get<uint16_t>();
    uint16_t latch_new = latch_cur;
    latch_new |= gain_mask[0][db_chan];
    if (latch_new > 0) {
        latch_new &= ~gain_mask[value][db_chan];
    }
    if (latch_new != latch_cur) {
        /*
         * The CFG_DAC is on the IO FPGA and read write accessable via
         * the host. Lock the module to restrict access and then hold
         * the bus lock once the port has been selected.
         */
        module::module::guard mod_guard(mod);
        mod.select_port(board.number.get<int>() + 1);
        module::module::bus_guard bus_guard(mod);
        auto chan_num = channel_gain.channel_.module_channel.number;
        if ((latch_new & 0xff) != (latch_cur & 0xff)) {
            const hw::word bits0_7 = latch_new & 0xff;
            hw::word word =
                (hw::i2c_device::mb_gain_pio_0 << 24) |
                (bits0_7 << 16) | (bits0_7 << 8) | bits0_7;
            log(log::debug) << pixie::module::module_label(mod, "gain")
                            << ": channel=" << chan_num
                            << " db-channel=" << db_chan
                            << std::hex << " lsb latch=0x" << bits0_7;
            mod.write_word(hw::device::CFG_DAC, word);
            hw::wait(4000);
        }
        if ((latch_new & 0xff00) != (latch_cur & 0xff00)) {
            const hw::word bits8_15 = (latch_new >> 8) & 0xff;
            hw::word word =
                (hw::i2c_device::mb_gain_pio_1 << 24) |
                (bits8_15 << 16) | (bits8_15 << 8) | bits8_15;
            log(log::debug) << pixie::module::module_label(mod, "gain")
                            << ": channel=" << chan_num
                            << " db-channel=" << db_chan
                            << std::hex << " msb latch=0x" << bits8_15;
            mod.write_word(hw::device::CFG_DAC, word);
            hw::wait(4000);
        }
        latch.unlock_writes();
        latch = latch_new;
        latch.lock_writes();
    }
}

void mb_gain_driver::enable_mibs() {
    steps.enable();
    latch.enable();
}

void mb_gain_driver::disable_mibs() {
    steps.disable();
    latch.disable();
}
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
