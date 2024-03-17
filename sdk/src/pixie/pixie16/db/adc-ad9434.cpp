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

/** @file adc-ad9434.cpp
 * @brief Implements AD9434 driver
 */

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/utils/string.hpp>

#include <pixie/pixie16/db/adc-ad9434.hpp>
#include <pixie/pixie16/db/adc-spi.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {

adc_channel_ad9434::adc_channel_ad9434(channel& channel__, adc_spi_driver& driver_)
    : channel_(channel__), driver(driver_) {
    xia::log(log::debug) << channel_label(channel_) << channel_.offset.get<size_t>() << ": adc init";
    label = channel_.get_mib_base() + "adc.ad9434";
    states.add("coupling:ac");
    states.add("fmt:offset-binary");
    states.add("invert:off");
    states.add("test-mode:off");
    state = mib::node(label, mib::type::string);
    state = states.get();
    state.lock_writes();
}

adc_channel_ad9434::~adc_channel_ad9434() {
    state.remove();
}

void adc_channel_ad9434::self_test() {
    auto chip_id = driver.read_reg(channel_, 0x01);
    auto chip_grade = driver.read_reg(channel_, 0x02);
    (void) chip_id;
    (void) chip_grade;
}

bool adc_channel_ad9434::self_test_pass() const {
    auto test = states.get("^test:");
    return test == "test:pass";
}

void adc_channel_ad9434::set_test_mode(channel::adc_test_mode mode) {
    hw::word val = 0;
    const char* test_mode_str = "test-mode:off";
    switch (mode) {
    default:
        break;
    case channel::adc_test_mode::fs_plus:
        test_mode_str = "test-mode:+FS";
        val = 2;
        break;
    case channel::adc_test_mode::fs_minus:
        test_mode_str = "test-mode:-FS";
        val = 3;
        break;
    case channel::adc_test_mode::midscale:
        test_mode_str = "test-mode:midscale";
        val = 1;
        break;
    case channel::adc_test_mode::checkerboard:
        test_mode_str = "test-mode:checkerboard";
        val = 4;
        break;
    case channel::adc_test_mode::pn23:
        test_mode_str = "test-mode:pn23";
        val = 5;
        break;
    case channel::adc_test_mode::pn9:
        test_mode_str = "test-mode:pn9";
        val = 6;
        break;
    case channel::adc_test_mode::one_zero:
        test_mode_str = "test-mode:one/zero";
        val = 7;
        break;
    }
    driver.write_reg(channel_, 0x0d, val);
    driver.write_reg(channel_, 0xff, 1);
    set_state("^test-mode:", test_mode_str);
}

void adc_channel_ad9434::set_ac_coupled() {
    driver.write_reg(channel_, 0x2c, 0);
    driver.write_reg(channel_, 0xff, 1);
    set_state("^coupling:", "coupling:ac");
}

void adc_channel_ad9434::set_dc_coupled() {
    driver.write_reg(channel_, 0x2c, 1 << 2);
    driver.write_reg(channel_, 0xff, 1);
    set_state("^coupling:", "coupling:dc");
}

void adc_channel_ad9434::set_invert_on() {
    driver.write_reg(channel_, 0x14, 1 << 2 | 0);
    driver.write_reg(channel_, 0xff, 1);
    set_state("^invert:", "invert:on");
}

void adc_channel_ad9434::set_invert_off() {
    driver.write_reg(channel_, 0x14, 0 << 2 | 0);
    driver.write_reg(channel_, 0xff, 1);
    set_state("^invert:", "invert:off");
}

void adc_channel_ad9434::enable_mibs() {
    state.enable();
}

void adc_channel_ad9434::disable_mibs() {
    state.disable();
}

void adc_channel_ad9434::set_state(const char* token, const std::string& value) {
    set_state(token, value.c_str());
}

void adc_channel_ad9434::set_state(const char* token, const char* value) {
    states.update(token, value);
    state.unlock_writes();
    state = states.get();
    state.lock_writes();
}

};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
