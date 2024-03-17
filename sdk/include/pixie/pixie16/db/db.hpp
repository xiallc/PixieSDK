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

#include <functional>

#include <pixie/log.hpp>
#include <pixie/mib.hpp>

#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/db/fippi-reg.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific daughter board fixtures.
 */
namespace fixture {
namespace db {

/**
 * @brief Daughter Board fixture
 */
struct db : public fixture::assembly {

    /**
     * The number is the position. The DBs are sorted by position so
     * asking for the index by channel returns the DB number.
     */
    mib::node number;

    /**
     * Base channel for the daughter board
     */
    mib::node base;

    /**
     * The number of channels the DB supports
     */
    mib::node channels;

    /**
     * FIPPI Registers
     */
    fippi::regs fippi_regs;

    db(pixie::module::module& module_, const hw::db_assembly& db_assembly);
    virtual ~db();

    virtual std::string get_mib_base() override;

    virtual void online() override;
    virtual void forced_online() override;
    virtual void forced_offline() override;

    template<typename T, typename B = db> void init_db_channels(B& board);

    virtual void enable_mibs();
    virtual void disable_mibs();
};

/**
 * @brief Daughter Board fixture
 */
struct channel : public fixture::channel {
    enum struct adc_test_mode {
        off,              /**< Disable test modes, default */
        fs_plus,          /**< +FS or +ve full scale */
        fs_minus,         /**< -FS or -ve full scale */
        midscale,         /**< Midscale of the range */
        checkerboard,     /**< Checkerboard or zero/one data */
        pn23,             /**< PN23 longitudinal test */
        pn9,              /**< PN9 longitudinal test */
        one_zero          /**< One/zero alternating data words */
    };
    /**
     * The daughter board the channel is on
     */
    db& board;

    /**
     * Channel offset relative to the fixture.
     */
    mib::node offset;

    /**
     * Dual ADC swapped state. This is `swapped` if the data is being clocked
     * on the wrong edge.
     */
    mib::node adc_swap;

    channel(
        pixie::channel::channel& module_channel, db& board,
        const hw::config& config_);
    virtual ~channel();

    virtual std::string get_mib_base();

    virtual void online() override;
    virtual void forced_online() override;
    virtual void forced_offline() override;

    virtual void enable_mibs();
    virtual void disable_mibs();

    virtual void acquire_adc() override;
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    virtual void set(const std::string item, bool value);
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, unsigned int& value);

    /**
     * Helpers for the ADC test mode and the `unsigned int` set
     * interface.
     */
    adc_test_mode get_adc_test_mode(unsigned int mode_value);
    bool check_adc_test_mode(unsigned int mode_value);
    const char* adc_mode_label(adc_test_mode mode);

};

/**
 * Uniform DB label for logging
 */
static inline std::string db_label(db& board) {
    return
        pixie::module::module_label(board.module_, "fixture") +
        board.label + '.' +
        std::to_string(board.number.get<int>()) + ": ";
}
/**
 * Uniform channel label for logging
 */
static inline std::string channel_label(channel& chan) {
    return
        chan.label + '.' +
        std::to_string(chan.board.number.get<int>()) + '.' +
        std::to_string(chan.module_channel.number) + ": ";
}

template<typename T, typename B> void db::init_db_channels(B& board) {
    log(log::debug) << db_label(board) << "init-channels: create channel fixtures";
    auto db_chans = channels.get<size_t>();
    auto db_base = base.get<size_t>();
    for (size_t db_chan = 0; db_chan < db_chans; ++db_chan) {
        auto& chan = module_.channels[db_base + db_chan];
        auto& config = module_.eeprom.configs[chan.number];
        chan.fixture = std::make_shared<T>(chan, board, config);
    }
}
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_DB_H
