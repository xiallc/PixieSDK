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

/** @file baseline.cpp
 * @brief Find the baseline of a channel from ADC trace data
 */

#include <algorithm>

#include <pixie/pixie16/baseline.hpp>

namespace xia {
namespace pixie {
namespace baseline {

channel::channel(int noise_bins_, double noise_percent_)
    : noise_bins(noise_bins_), noise_percent(noise_percent_), number(-1),
      adc_bits(0), runs(0), baseline(-1) {
    if (noise_bins > 500) {
        noise_bins = 500;
    }
    if (noise_percent > 100) {
        noise_percent = 100;
    } else if (noise_percent < 0) {
        noise_percent = 0;
    }
}

void channel::start(size_t number_, int adc_bits_) {
    number = number_;
    adc_bits = adc_bits_;
    bins.clear();
    bins.resize(bin_buckets::size_type(1) << adc_bits);
    runs = 0;
    baseline = -1;
}

void channel::end() {
    /*
     * Find the bin with the most values, the signal spent the most time at
     * that voltage. Average a number of bins either side;
     */
    auto max = std::max_element(bins.begin() + noise_bins, bins.end() - noise_bins);
    int max_bin = int(std::distance(bins.begin(), max));
    int from_bin;
    int to_bin;
    if (bins[max_bin] != 0) {
        from_bin = std::max(max_bin - noise_bins, 0);
        to_bin = std::min(max_bin + noise_bins, int(bins.size()));
    } else {
        auto max_top = std::max_element(bins.end() - noise_bins, bins.end());
        int max_top_bin = int(std::distance(bins.begin(), max_top));
        auto max_bottom = std::max_element(bins.begin(), bins.begin() + noise_bins);
        int max_bottom_bin = int(std::distance(bins.begin(), max_bottom));
        if (bins[max_top_bin] > bins[max_bottom_bin]) {
            from_bin = int(bins.size()) - noise_bins;
            to_bin = int(bins.size());
        } else {
            from_bin = 0;
            to_bin = noise_bins;
        }
    }
    int sum = 0;
    int samples = 0;
    for (int b = from_bin; b < to_bin; ++b) {
        sum += b * bins[b];
        samples += bins[b];
    }
    if (samples > 0) {
        baseline = sum / samples;
    } else {
        baseline = 0;
    }
}

void channel::update(const hw::adc_trace& trace) {
    ++runs;
    for (auto sample : trace) {
        if (sample >= bins.size()) {
            sample = static_cast<unsigned short>(bins.size() - 1);
        }
        ++bins[sample];
    }
}

bool channel::operator==(const channel& other) const
{
    return *this == other.baseline;
}

bool channel::operator!=(const channel& other) const
{
    return *this != other.baseline;
}

bool channel::operator==(const int bl) const
{
    int range = 1;
    if (noise_percent > 0) {
        range = int((1 << adc_bits) * (noise_percent / 100));
    }
    return baseline >= (bl - range) && baseline <= (bl + range);
}

bool channel::operator!=(const int bl) const
{
    return !(*this == bl);
}

};  // namespace fixture
};  // namespace pixie
};  // namespace xia
