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

/** @file db10.cpp
 * @brief Implements DB10
 */

#include <algorithm>
#include <cstring>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/db10.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

static size_t bitslip_chan_base[4] = { 0, 4, 12, 8 };

db10::db10(pixie::module::module& module_, const hw::db_assembly& db_assembly)
    : db(module_, db_assembly), adc(*this), gain(*this) {
    fippi::make(fippi_regs, "bitslip", *this, hw::fippi::BITSLIP);
}

db10::~db10() {
}

void db10::init_channels() {
    init_db_channels<db10_channel>(*this);
}

void db10::boot() {
    auto base_ = base.get<size_t>();
    auto channels_ = channels.get<size_t>();
    for (size_t offset = 0; offset < channels_; ++offset) {
        module_.channels[base_ + offset].fixture->event("adc.init");
    }
}

void db10::enable_mibs() {
    db::enable_mibs();
    gain.enable_mibs();
}

void db10::disable_mibs() {
    db::disable_mibs();
    gain.disable_mibs();
}

db10_channel::db10_channel(
    pixie::channel::channel& mod_chan, db10& board, const hw::config& config_)
    : channel(mod_chan, board, config_),
      adc(*this, board.adc), gain(*this, board.gain), dac(*this) {
    adc_swap = "disabled";
    bitslip_reg = fippi::find("bitslip", board.fippi_regs);
    bitslip = mib::node(get_mib_base() + "bitslip", mib::type::uinteger);
    bitslip = 0;
    mib::event_func bitslip_set =
        [&self = *this, db_chan = offset.get<size_t>()]
        (mib::event , mib::type , mib::data_type& data) {
            data.u &= 0xf;
            self.bitslip_reg->write(
                static_cast<hw::word>(data.u), bitslip_chan_base[db_chan], 4);
        };
    bitslip.set_event_func(mib::event::set, bitslip_set);
}

db10_channel::~db10_channel() {
    bitslip.clear_event_func(mib::event::set);
    bitslip.remove();
}

void db10_channel::set_dac(param::value_type value) {
    dac.set_dac(value);
}

void db10_channel::event(const std::string name) {
    if (name == "adc.init") {
        adc.set_dc_coupled();
    } else {
        channel::event(name);
    }
}

void db10_channel::set(const std::string item, unsigned int value) {
    if (item == "adc.test-mode") {
        if (check_adc_test_mode(value)) {
            adc.set_test_mode(get_adc_test_mode(value));
        } else {
            auto& mod = get_module();
            throw pixie::module::error(
                mod.number, mod.slot, pixie::module::error::code::invalid_value,
                "invalid ADC test mode");
        }
    } else if (item == "adc.test-mode.result") {
        std::string result = "test:";
        if (value == 0) {
            result += "pass";
        } else {
            result += "fail:";
            result += adc_mode_label(get_adc_test_mode(value));
        }
        adc.set_state("^test:", result);
    } else if (item == "adc.bitslip") {
        bitslip.set(value);
    } else {
        channel::set(item, value);
    }
}

void db10_channel::get(const std::string item, bool& value) {
    if (item == "dac.has.offset" ||
        item == "adc.has.self-test" ||
        item == "adc.has.test-mode" ||
        item == "adc.has.bitslip") {
        value = true;
    } else if (item == "adc.default.inverted") {
        value = true;
    } else if (item == "adc.test-mode.result.pass") {
        value = adc.self_test_pass();
    } else if (item == "adc.swap.disable") {
        value = true;
    } else {
        channel::get(item, value);
    }
}

void db10_channel::get(const std::string item, unsigned int& value) {
    if (item == "adc.bitslip.bits") {
        value = 16;
    } else if (item == "dac.settle-period") {
        value = dac.settle_time_ms;
    } else {
        channel::get(item, value);
    }
}

void db10_channel::enable_mibs() {
    channel::enable_mibs();
    adc.enable_mibs();
    gain.enable_mibs();
    dac.enable_mibs();
    bitslip.enable();
}

void db10_channel::disable_mibs() {
    channel::disable_mibs();
    adc.disable_mibs();
    gain.disable_mibs();
    dac.disable_mibs();
    bitslip.disable();
}
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
