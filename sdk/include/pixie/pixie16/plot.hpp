/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2025 XIA LLC, All rights reserved.
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

/** @file plot.hpp
 * @brief Plots data as an ASCII graph. Useful for simple checks of
 * signals.
 */

#ifndef PIXIE_ACQ_PLOT_H
#define PIXIE_ACQ_PLOT_H

#ifdef XIA_PIXIE_WINDOWS
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <functional>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace plot {

using write_string_func = std::function<void(const std::string& out)>;

/**
 * @brief ASCII Plot
 */
template <typename T>
void ascii(
    const std::vector<T>& data, write_string_func writer,
    size_t cols, size_t rows) {
    T r_min = std::numeric_limits<T>::max();
    T r_max = 0;
    size_t r_unit = 0;
    size_t x_unit;
    for (auto sample : data) {
        r_max = std::max(r_max, sample);
        r_min = std::min(r_min, sample);
    }
    x_unit = data.size() / cols;
    r_unit = static_cast<size_t>((r_max - r_min) / (rows - 1));
    for (auto r = static_cast<ssize_t>(rows); r >= 0; --r) {
        std::ostringstream oss;
        T r_top = T(r * r_unit) + r_min;
        T r_bot = r_top - T(r_unit);
        char dot = ' ';
        oss << std::setw(7) << r_top << " |";
        for (size_t s = 0; s < data.size(); ++s) {
            if ((data[s] >= r_bot) && (data[s] < r_top)) {
                dot = 'o';
            }
            if (s && ((s % x_unit) == 0)) {
                oss << dot;
                dot = ' ';
            }
        }
        oss << std::endl;
        writer(oss.str());
    }
    writer("         +");
    std::string dashes(cols, '-');
    writer(dashes);
}
}  // namespace plot
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_ACQ_PLOT_H
