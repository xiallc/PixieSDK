/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
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

/** @file db04.hpp
 * @brief Defines DB04 fixture
 */

#ifndef PIXIE_DB_DB04_H
#define PIXIE_DB_DB04_H

#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief DB04 fixture
 */
struct db04 : public fixture::db::db {
    /*
     * The DAC output has a filter with an RC 1/e settling time of 47
     * ms. Lets wait a after setting it for the signal to settle.
     */
    static constexpr int dac_settle_time_ms = 250;

    db04(pixie::module::module& module_, const hw::db_assembly& db_assembly);
    virtual ~db04();

    virtual void init_channels() override;
};

/**
 * @brief DB04 fixture
 */
struct db04_channel : public fixture::db::channel {
    db04_channel(
        pixie::channel::channel& module_channel, db& board, const hw::config& config_);
    virtual ~db04_channel();

    virtual void set_dac(param::value_type value) override;

    virtual void get(const std::string item, bool& value) override;
    virtual void get(const std::string item, unsigned int& value) override;
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_DB04_H
