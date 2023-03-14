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

#ifndef PIXIE_DB_H
#define PIXIE_DB_H

#include <pixie/pixie16/baseline.hpp>
#include <pixie/pixie16/fixture.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific daughter board fixtures.
 */
namespace fixture {
/**
 * @brief Daughter Board fixture
 */
struct db : public channel {
    /**
     * ADC swapped state.
     *
     * If the DSP is not loaded the swap state is unknown.
     */
    enum adc_swap_state {
        adc_swap_disabled,
        adc_boot_state,
        adc_unswapped,
        adc_swapped
    };
    /**
     * The number is the position. The DBs are sorted by position so
     * asking for the index by channel returns the DB number.
     */
    int number;

    /**
     * Base channel for the daughter board
     */
    int base;

    /**
     * Channel offset relative to the fixture.
     */
    int offset;

    /**
     * Dual ADC swapped state. This is true if the data is being clocked
     * on the wrong edge.
     */
    adc_swap_state adc_state;

    db(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void acquire_adc() override;
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    virtual void set(const std::string item, bool value);
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, int& value);
    virtual void get(const std::string item, double& value);

    virtual void report(
        std::ostream& out, const std::string& prefix = "") const override;
};

/**
 * @brief Pixie16 module's have AFE daughter board fixtures
 */
struct afe_dbs : public module {
    enum limits {
        max_dbs = 4
    };

    static constexpr int baseline_noise_bins = 30;
    static constexpr double baseline_noise_margin = 0.5; /* % */

    /*
     * These are development modes.
     */
    const bool adc_swap_verify = true;

    std::array<hw::word, max_dbs> adcctrl;

    afe_dbs(pixie::module::module& module_);

    virtual void fgpa_fippi_loaded() override;
    virtual void boot() override;
    virtual void init_channels() override;

    virtual void set_dacs() override;
    virtual void get_traces() override;
    virtual void adjust_offsets() override;

    void analyze_channel_baselines(
        baseline::channels& baselines, const int traces = 1);
};

/**
 * @brief DB04 fixture
 */
struct db04 : public db {
    /*
     * The DAC output has a filter with an RC 1/e settling time of 47
     * ms. Lets wait a after setting it for the signal to settle.
     */
    const int dac_settle_time_ms = 250;

    db04(pixie::channel::channel& module_channel_, const hw::config& config_);

    virtual void set_dac(param::value_type value) override;

    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, int& value);
};

}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_H
