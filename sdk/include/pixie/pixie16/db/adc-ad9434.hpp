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

/** @file adc-ad9435.hpp
 * @brief Defines ADC driver for the AD9434
 */

#ifndef PIXIE_DB_ADC_AD9434_H
#define PIXIE_DB_ADC_AD9434_H

#include <pixie/utils/string.hpp>

#include <pixie/pixie16/db/adc-spi.hpp>
#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief Daughter Board ADC Driver for AD9434
 */
struct adc_channel_ad9434 {
    channel& channel_;
    adc_spi_driver& driver;
    std::string label;
    xia::mib::node state;
    util::string::token_editor states;
    adc_channel_ad9434(channel& channel_, adc_spi_driver& driver);
    ~adc_channel_ad9434();
    void self_test();
    bool self_test_pass() const;
    void set_test_mode(channel::adc_test_mode mode);
    void set_ac_coupled();
    void set_dc_coupled();
    void set_invert_on();
    void set_invert_off();
    void enable_mibs();
    void disable_mibs();
    void set_state(const char* token, const std::string& value);
    void set_state(const char* token, const char* value);
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_ADC_AD9434_H
