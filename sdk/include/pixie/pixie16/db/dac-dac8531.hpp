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

/** @file dac-dac8531.hpp
 * @brief Defines DAC driver for the IT DAC8531
 */

#ifndef PIXIE_DB_DAC_DAC8531_H
#define PIXIE_DB_DAC_DAC8531_H

#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief Daughter Board DAC Driver for DAC8531
 */
struct dac_dac8531 {
    static constexpr size_t bits = 16;
    static constexpr size_t settle_time_ms = 100;
    channel& channel_;
    std::string label;
    hw::address addr;
    xia::mib::node value;
    dac_dac8531(channel& channel_);
    ~dac_dac8531();
    void set_dac(param::value_type value);
    void enable_mibs();
    void disable_mibs();
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_DAC_DB851_H
