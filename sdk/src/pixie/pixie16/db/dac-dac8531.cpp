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

/** @file dac-dac8531.cpp
 * @brief Implements DAC8531 driver
 */

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/dac-dac8531.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

dac_dac8531::dac_dac8531(channel& channel__)
    : channel_(channel__), addr(0) {
    xia::log(log::info) << channel_label(channel_) << "dac init";
    addr = hw::fippi_addr(
        channel_.board.number.get<hw::address>(),
        channel_.offset.get<hw::address>(),
        hw::fippi::FIPPI_DAC);
    label = channel_.get_mib_base() + "dac";
    value = mib::node(label, mib::type::integer);
    mib::event_func setter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            self.set_dac(data.get<param::value_type>());
        };
    value.set_event_func(mib::event::set, setter);
}

dac_dac8531::~dac_dac8531() {
    value.remove();
}

void dac_dac8531::set_dac(param::value_type val) {
    auto& mod = channel_.module_channel.module.get();
    if (val > (1 << bits)) {
        throw error::error(
            error::code::invalid_value,
            label +  ": invalid DAC offset: value=" + std::to_string(val));
    }
    value = val;
    /*
     * Mode: PD1/PD0 set to 0 for normal oeration.
     */
    const hw::word dac = val << 1;
    log(log::debug) << label
                    << std::hex
                    << ": value=0x" << val
                    << " write=0x" << dac << " => 0x" << addr;
    hw::memory::fippi fippi(mod);
    fippi.write(addr, dac);
     /*
     * It takes about 4ms to clock out the 32 bits.
     */
    hw::wait(6000);
}

void dac_dac8531::enable_mibs() {
    value.enable();
}

void dac_dac8531::disable_mibs() {
    value.disable();
}
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
