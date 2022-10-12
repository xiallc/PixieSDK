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

/** @file baseline.hpp
 * @brief Find a baseline for a channel
 */

#ifndef PIXIE_BASELINE_H
#define PIXIE_BASELINE_H

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Finds a Pixie-16 channel's baseline from ADC trace data.
 */
namespace baseline {
/**
 * @brief The channel's baseline.
 */
/**
 * ADC trace analyize
 *
 * This is a simple approach to detecting the average voltage.
 */
struct channel {
    using bin_buckets = std::vector<int>;

    int noise_bins;
    double noise_percent;
    int number;
    int adc_bits;

    int runs;
    int baseline;

    channel(int noise_bins = 30, double noise_percent = 0.5);

    void start(int channel, int adc_bits);
    void end();

    void update(const hw::adc_trace& trace);

    bool operator==(const channel& other) const;
    bool operator!=(const channel& other) const;
    bool operator==(const int bl) const;
    bool operator!=(const int bl) const;

    bin_buckets bins;
};

using channels = std::vector<channel>;

}  // namespace baseline
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_BASELINE_H
