/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file numerics.hpp
 * @brief Defines utility functions and data structures related to numerics useful throughout the SDK.
 */

#ifndef PIXIESDK_UTIL_NUMERICS_HPP
#define PIXIESDK_UTIL_NUMERICS_HPP

#include <climits>
#include <cmath>
#include <limits>
#include <vector>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace numerics {

/**
 * @brief Defines a type for the IEEE 754 floating point standard.
 */
struct ieee_float {
    using value_type = unsigned int;

    ieee_float();
    ieee_float(const ieee_float& ieee);
    ieee_float(value_type value_);
    ieee_float(double dec_num);

    ieee_float& operator=(const ieee_float& ieee);
    ieee_float& operator=(const double dec_num);

    bool operator==(const ieee_float& ieee) const;
    bool operator==(const double dec_num) const;
    bool operator==(const value_type num) const;

    operator double() const;
    operator value_type() const;

private:
    value_type in(const double dec_num) const;
    double out() const;

    value_type value;
};

/**
 * Average a series of numbers and record the maximum and minimum values.
 */
template<typename T>
struct average {
    T avg;
    T max;
    T min;
    size_t count;
    average() :
        avg(0),
        max(std::numeric_limits<T>::min()),
        min(std::numeric_limits<T>::max()), count(0) {}
    void update(T val) {
        avg += val;
        max = std::max(max, val);
        min = std::min(min, val);
        ++count;
    }
    void calc() { if (count > 0) avg /= count; }
};

/**
 * Variance is average of the squared differences from the mean.
 */
template<typename T>
struct variance {
    double var;
    double stddev;
    double mean;
    size_t count;
    variance(double mean_) : var(0), stddev(0), mean(mean_), count(0) {}
    void update(T val) {
        val -= mean;
        var += val * val;
        ++count;
    }
    void calc() {
        if (count > 0) {
            var /= count;
        }
        stddev = std::sqrt(var);
    }
};

/**
 * linear fit using least squares. Provide linear interpolation.
 */
template<typename T>
struct linear_fit {
    using sample = std::pair<T, T>;
    std::vector<sample> samples;

    /*
     * Y = kX + c
     */
    double k;
    double c;

    double sum_x;
    double sum_y;
    double sum_xy;
    double sum_x_sq;

    int count;

    linear_fit() : k(0), c(0), sum_x(0), sum_y(0), sum_xy(0), sum_x_sq(0), count(0) {}

    void update(T x, T y) {
        samples.push_back(std::make_pair(x, y));
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x_sq += x * x;
        ++count;
    }

    void calc() {
        auto divisor = (sum_x * sum_x) - (count * sum_x_sq);
        k = ((sum_x * sum_y) - (count * sum_xy)) / divisor;
        c = ((sum_x * sum_xy) - (sum_y * sum_x_sq)) / divisor;
    }

    T y(T x) {
        return T((k * x) + c);
    }
};

} // namespace numerics
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_NUMERICS_HPP
