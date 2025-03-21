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

enum adc_regs {
    test_mode = 0x0d,
    fmt = 0x14,
    coupling = 0x2c
};

adc_channel_ad9434::adc_channel_ad9434(channel& channel__, adc_spi_driver& driver_)
    : channel_(channel__), driver(driver_) {
    xia::log(log::debug) << channel_label(channel_) << channel_.offset.get<size_t>() << ": adc init";
    label = channel_.get_mib_base() + "adc.ad9434";
    active_reg = mib::node(label + mib::mibsep + "reg", mib::type::integer);
    active_reg = 0x00;
    spi_value = mib::node(label + mib::mibsep + "value", mib::type::integer);
    spi_value = 0;
    mib::event_func spi_getter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            auto val = self.driver.read_reg(self.channel_,
                                            self.active_reg.get<param::value_type>());
            data.u = val;
        };
    mib::event_func spi_setter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            self.write_adc_setting(self.active_reg.get<param::value_type>(),
                                   data.get<param::value_type>());
        };
    spi_value.set_event_func(mib::event::get, spi_getter);
    spi_value.set_event_func(mib::event::set, spi_setter);
    states.add("coupling:ac");
    states.add("fmt:offset-binary");
    states.add("invert:off");
    states.add("test-mode:off");
    state = mib::node(label, mib::type::string);
    state = states.get();
    sync_adc_settings();
    state.lock_writes();
}

adc_channel_ad9434::~adc_channel_ad9434() {
    active_reg.remove();
    spi_value.remove();
    state.remove();
}

void adc_channel_ad9434::self_test() {
    auto chip_id = driver.read_reg(channel_, 0x01);
    auto chip_grade = driver.read_reg(channel_, 0x02);
    bool pass = true;
    if (chip_id == 0x6A) {
        std::string id = "id:ad9434-";
        auto grade = (chip_grade >> 4);
        switch (grade) {
            case 1:
                id += "370";
                break;
            case 2:
                id += "500";
                break;
            default:
                id += "unknown-" + std::to_string(grade);
                pass = false;
                break;
        }
        states.add(id);
    } else {
        states.add("id:unknown-" + std::to_string(chip_id));
        pass = false;
    }
    if (pass) {
        states.add("test:pass");
    } else {
        states.add("test:fail");
    }
}

bool adc_channel_ad9434::self_test_pass() const {
    auto test = states.get("^test:");
    return test == "test:pass";
}

hw::word adc_channel_ad9434::bits_from_test_mode(channel::adc_test_mode mode) {
    switch (mode) {
        case channel::adc_test_mode::fs_plus:
            return 2;
        case channel::adc_test_mode::fs_minus:
            return 3;
        case channel::adc_test_mode::midscale:
            return 1;
        case channel::adc_test_mode::checkerboard:
        case channel::adc_test_mode::pn23:
        case channel::adc_test_mode::pn9:
        case channel::adc_test_mode::one_zero:
            return static_cast<hw::word>(mode);
        default:
            return 0;
    }
};

channel::adc_test_mode adc_channel_ad9434::test_mode_from_bits(hw::word bits) {
    switch (bits) {
        case 1:
            return channel::adc_test_mode::midscale;
        case 2:
            return channel::adc_test_mode::fs_plus;
        case 3:
            return channel::adc_test_mode::fs_minus;
        default:
            return channel_.get_adc_test_mode(bits);
    }
};

void adc_channel_ad9434::set_test_mode(channel::adc_test_mode mode) {
    std::string test_mode_str = "test-mode:";
    test_mode_str += channel_.adc_mode_label(mode);
    hw::word val = bits_from_test_mode(mode);
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
    auto r14 = driver.read_reg(channel_, 0x14);
    driver.write_reg(channel_, 0x14, r14 | (1 << 2));
    driver.write_reg(channel_, 0xff, 1);
    set_state("^invert:", "invert:on");
}

void adc_channel_ad9434::set_invert_off() {
    auto r14 = driver.read_reg(channel_, 0x14);
    driver.write_reg(channel_, 0x14, r14 & ~(1 << 2));
    driver.write_reg(channel_, 0xff, 1);
    set_state("^invert:", "invert:off");
}

void adc_channel_ad9434::set_format(param::value_type fmt) {
    auto r14 = driver.read_reg(channel_, 0x14);
    r14 &= ~(0x3 << 0);
    driver.write_reg(channel_, 0x14, r14 | (fmt << 0));
    driver.write_reg(channel_, 0xff, 1);
    switch (fmt) {
        case 0:
            set_state("^fmt:", "fmt:offset-binary");
            break;
        case 1:
            set_state("^fmt:", "fmt:twos-complement");
            break;
        case 2:
            set_state("^fmt:", "fmt:gray-code");
            break;
        default:
            set_state("^fmt:", "fmt:invalid");
    }
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

void adc_channel_ad9434::write_adc_setting(param::value_type reg, param::value_type val) {
    if (reg == test_mode) {
        set_test_mode(test_mode_from_bits(val));
    } else if (reg == fmt) {
        if ((val >> 2) & 1) {
            set_invert_on();
        } else {
            set_invert_off();
        }
        set_format(val & 3);
    } else if (reg == coupling) {
        if (val == 0) {
            set_ac_coupled();
        } else {
            set_dc_coupled();
        }
    } else {
        driver.write_reg(channel_, reg, val);
        driver.write_reg(channel_, 0xff, 1);
    }
}

void adc_channel_ad9434::sync_adc_settings() {
    if (channel_.get_module().hardware_accessible()) {
        std::string test_mode_str = "test-mode:";
        auto val = driver.read_reg(channel_, test_mode);
        auto mode = test_mode_from_bits(val);
        set_state("^test-mode:", test_mode_str + channel_.adc_mode_label(mode));

        val = driver.read_reg(channel_, fmt);
        if ((val >> 2) & 1) {
            set_state("^invert:", "invert:on");
        } else {
            set_state("^invert:", "invert:off");
        }
        auto fmt = val & 3;
        if (fmt == 0) {
            set_state("^fmt:", "fmt:offset-binary");
        } else if (fmt == 1) {
            set_state("^fmt:", "fmt:twos-complement");
        } else if (fmt == 2) {
            set_state("^fmt:", "fmt:gray-code");
        } else {
            set_state("^fmt:", "fmt:invalid");
        }

        val = driver.read_reg(channel_, coupling);
        if (val == 0) {
            set_state("^coupling:", "coupling:ac");
        } else {
            set_state("^coupling:", "coupling:dc");
        }
    }
}

};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
