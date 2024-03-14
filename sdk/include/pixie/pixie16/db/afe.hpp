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

#ifndef PIXIE_DB_AFE_H
#define PIXIE_DB_AFE_H

#include <pixie/pixie16/baseline.hpp>
#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific daughter board fixtures.
 */
namespace fixture {
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

    virtual void fpga_fippi_loaded() override;
    virtual void boot() override;
    virtual void init_channels() override;

    virtual void set_dacs() override;
    virtual void get_traces() override;
    virtual void adjust_offsets() override;

    void analyze_channel_baselines(
        baseline::channels& baselines, const int traces = 1);
};
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_AFE_H
