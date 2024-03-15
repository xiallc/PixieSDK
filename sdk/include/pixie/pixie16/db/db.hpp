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

/** @file db.hpp
 * @brief Defines daughter board fixtures
 */

#ifndef PIXIE_DB_DB_H
#define PIXIE_DB_DB_H

#include <pixie/mib.hpp>

#include <pixie/pixie16/fixture.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific daughter board fixtures.
 */
namespace fixture {
struct db;
/**
 * DB MIB
 */
struct db_mib {
    std::string item;
    mib::node node;
    db_mib();
    void register_mib(db& db_, const std::string& mib_name, mib::type type = mib::type::integer);
    void register_ro_mib(const std::string& name, int value);
    void setter(db& db_, mib::type type, mib::data_type& data);
    void getter(db& db_, mib::type type, mib::data_type& data);
    mib::node& operator*() { return node; }
    mib::node* operator->() { return &node; }
};

/**
 * @brief Daughter Board fixture
 */
struct db : public channel {
    /**
     * The number is the position. The DBs are sorted by position so
     * asking for the index by channel returns the DB number.
     */
    db_mib number;

    /**
     * Base channel for the daughter board
     */
    db_mib base;

    /**
     * Channel offset relative to the fixture.
     */
    db_mib offset;

    /**
     * Dual ADC swapped state. This is true if the data is being clocked
     * on the wrong edge.
     */
    db_mib adc_state;

    db(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void acquire_adc() override;
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    virtual void set(const std::string item, bool value);
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, int& value);
    virtual void get(const std::string item, double& value);
};
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_DB_H
