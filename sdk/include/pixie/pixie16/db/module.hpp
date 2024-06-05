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

/** @file module.hpp
 * @brief Defines daughter board module fixture
 */

#ifndef PIXIE_DB_AFE_H
#define PIXIE_DB_AFE_H

#include <pixie/pixie16/baseline.hpp>
#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief Pixie16 module's assembly supports AFE daughter board
 * fixtures
 */
struct module : public fixture::assembly {
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

    /**
     * @brief Test by setting the testing MIB string with `self`,
     * `adc`.
     */
    mib::node testing;

    module(pixie::module::module& module_);
    virtual ~module();

    virtual void fpga_fippi_loaded() override;
    virtual void boot() override;

    virtual void init_assemblies() override;

    virtual void set_dacs() override;
    virtual void get_traces() override;
    virtual void adjust_offsets() override;

    virtual bool has_test(const std::string text) override;
    virtual void run_test(
        const std::string test, bool& result, log_output& out) override;

    void analyze_channel_baselines(
        baseline::channels& baselines, const int traces = 1);

    void adc_self_test(bool use_log = true);
    bool adc_pattern_check(log_output& out);
    bool adc_sweet_spot(log_output& out);
    bool adc_channel_swap(log_output& out);

    /**
     * Check for a feature being enabled on all channels.
     */
    bool check_channel_feature(std::string item);
    /**
     * Check for a feature being enabled on a channel.
     */
    bool check_channel_feature(std::string item, size_t channel);
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_MODULE_H
