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

/** @file ieee-float.cpp
 * @brief Defines utility functions and data structures related to ieee-float values useful
 *        throughout the SDK.
 */

#include <cmath>

#include <pixie/utils/numerics.hpp>

namespace xia {
namespace util {
namespace numerics {
ieee_float::ieee_float() : value(0) {}

ieee_float::ieee_float(const ieee_float& ieee) : value(ieee.value) {}

ieee_float::ieee_float(value_type value_) : value(value_) {}

ieee_float::ieee_float(const double dec_num) : value(in(dec_num)) {}

ieee_float& ieee_float::operator=(const ieee_float& ieee) {
    value = ieee.value;
    return *this;
}

ieee_float& ieee_float::operator=(const double dec_num) {
    value = in(dec_num);
    return *this;
}

bool ieee_float::operator==(const ieee_float& ieee) const {
    return value == ieee.value;
}

bool ieee_float::operator==(const double dec_num) const {
    return value == in(dec_num);
}

bool ieee_float::operator==(const value_type num) const {
    return value == num;
}

ieee_float::operator double() const {
    return out();
}

ieee_float::operator value_type() const {
    return value;
}

ieee_float::value_type ieee_float::in(const double dec_num) const {
    if (dec_num == 0) {
        return 0;
    }

    /*
     * Get the absolute value then convert the integer part.
     */
    double abs_dec_num = std::fabs(dec_num);
    value_type int_part = static_cast<value_type>(std::floor(abs_dec_num));
    value_type int_part_hex = 0;
    size_t k;
    for (k = 0; k < 32 && int_part > 0; ++k, int_part >>= 1) {
        if ((int_part & 1) == 1) {
            int_part_hex |= 1 << k;
        } else {
            int_part_hex &= ~(1 << k);
        }
    }

    /*
     * Get the fractional part then convert it.
     */
    double frac_part = abs_dec_num - std::floor(abs_dec_num);
    value_type frac_part_hex = 0;
    size_t m;
    for (m = 0; m < 32 && frac_part > 0; ++m) {
        if ((frac_part * 2.0) >= 1.0) {
            frac_part_hex |= 1 << (31 - m);
            frac_part *= 2.0;
            frac_part -= 1.0;
        } else {
            frac_part_hex &= ~(1 << (31 - m));
            frac_part *= 2.0;
        }
    }

    value_type result;

    if (dec_num > 0) {
        result = 0;
    } else {
        result = 1 << 31;
    }

    /*
     * Combine the integer and fractional part. First, find the exponent
     */
    if (int_part_hex > 0) {
        /*
         * 127 is the exponent bias
         */
        value_type exponent = value_type(127 + k - 1);

        result += exponent * (1 << 23);

        /*
         * Add the mantissa
         */
        int_part_hex &= ~(1 << (k - 1));
        result += int_part_hex * (1 << (22 - (k - 2)));
        result += frac_part_hex >> (32 - (23 - (k - 1)));
    } else {
        /*
         * Search the fist non-zero bit in fract_part_hex
         */
        int n;
        for (n = 31; n > 0; --n) {
            if ((frac_part_hex & (1 << n)) != 0) {
                break;
            }
        }

        /*
         * 127 is the exponent bias
         */
        value_type exponent = 127 - (32 - n);

        result += exponent * (1 << 23);

        /*
         * Add the mantissa
         */
        frac_part_hex &= ~(1 << n);
        if (n >= 23) {
            result += frac_part_hex >> (n - 23);
        } else {
            result += frac_part_hex << (32 - n);
        }
    }

    return result;
}

double ieee_float::out() const {
    auto signbit = static_cast<short>(value >> 31);
    auto exponent = static_cast<short>(((value & 0x7F800000) >> 23) - 127);
    double mantissa = 1.0 + ((double(value & 0x7FFFFF) / double(1 << 23)));
    double result = mantissa * std::pow(2.0, static_cast<double>(exponent));
    if (signbit != 0)
        result *= -1.0;
    return result;
}
}
}
}
