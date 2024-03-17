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

/** @file mb-gain.hpp
 * @brief Defines motherboard gain control
 */

#ifndef PIXIE_DB_MB_GAIN_H
#define PIXIE_DB_MB_GAIN_H

#include <memory>
#include <mutex>

#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
struct mb_gain_driver;
/**
 * @brief Motherboard Gain Control
 * There is an instance per daughter board that acesses a
 * single driver instance that is concurrent.
 */
struct mb_channel_gain {
    channel& channel_;
    mb_gain_driver& driver;
    xia::mib::node level;
    mb_channel_gain(channel& channel_, mb_gain_driver& driver);
    ~mb_channel_gain();
    void init(channel& channel_);
    void set_gain(param::value_type value);
    void enable_mibs();
    void disable_mibs();
};

/**
 * @brief Motherboard Gain Driver
 * There is an instance per daughter board that acesses a
 * single driver instance that is concurrent.
 */
struct mb_gain_driver {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;
    db& board;
    xia::mib::node steps;
    xia::mib::node latch;
    lock_type lock;
    mb_gain_driver(db& board);
    ~mb_gain_driver();
    void set_gain(mb_channel_gain& channel_gain, param::value_type value);
    void enable_mibs();
    void disable_mibs();
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_MB_GAIN_H
