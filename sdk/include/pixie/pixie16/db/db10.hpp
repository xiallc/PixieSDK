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

/** @file db10.hpp
 * @brief Defines DB10 fixture
 */

#ifndef PIXIE_DB_DB10_H
#define PIXIE_DB_DB10_H

#include <pixie/pixie16/db/adc-ad9434.hpp>
#include <pixie/pixie16/db/adc-spi.hpp>
#include <pixie/pixie16/db/db.hpp>
#include <pixie/pixie16/db/dac-dac8531.hpp>
#include <pixie/pixie16/db/mb-gain.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief DB10 fixture
 */
struct db10 : public db {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;

    adc_spi_driver adc;
    mb_gain_driver gain;

    lock_type lock;

    db10(pixie::module::module& module_, const hw::db_assembly& db_assembly);
    virtual ~db10();

    virtual void init_channels() override;

    virtual void boot() override;

    virtual void enable_mibs() override;
    virtual void disable_mibs() override;

    void bitslip_write(size_t db_chan, size_t bits);
};

/**
 * @brief DB10 channel fixture
 */
struct db10_channel : public channel {
    adc_channel_ad9434 adc;
    mb_channel_gain gain;
    dac_dac8531 dac;

    /**
     * BITSLIP - Bitslip or input delays for ADC data, range 0-15
     *
     *  0-3  clk delay ch.0
     *  4-7  clk delay ch.1
     *  8-11 clk delay ch.3
     * 12-15 clk delay ch.2
     */
    fippi::reg_ptr bitslip_reg;
    mib::node bitslip;

    db10_channel(
        pixie::channel::channel& module_channel, db10& board, const hw::config& config_);
    virtual ~db10_channel();

    virtual void set_dac(param::value_type value) override;

    virtual void event(const std::string name) override;
    virtual void set(const std::string item, unsigned int value) override;
    virtual void get(const std::string item, bool& value) override;
    virtual void get(const std::string item, unsigned int& value) override;

    virtual void enable_mibs() override;
    virtual void disable_mibs() override;
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_DB10_H
