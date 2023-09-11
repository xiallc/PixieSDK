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

/** @file io.hpp
 * @brief Defines utility functions and data structures related to IO and streams useful throughout
 *        the SDK.
 */

#ifndef PIXIESDK_UTIL_IO_HPP
#define PIXIESDK_UTIL_IO_HPP

#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <pixie/utils/string.hpp>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace io {
/**
 * @brief Save and restore the output stream's settings.
 */
struct ostream_guard {
    std::ostream& o;
    std::ios_base::fmtflags flags;
    ostream_guard(std::ostream& o_) : o(o_), flags(o_.flags()) {}
    ~ostream_guard() {
        o.flags(flags);
    }
};

/**
 * @brief Get the value from the option. Raise an error if
 *        a value cannot be extracted form the option.
 * @tparam T The type of value to get from the option and return
 * @param[in] opt The option to get the value from.
 * @returns The value that we extract from the option
 */
template<typename T>
static T get_value(const std::string& opt) {
    T value = 0;
    try {
        if (std::is_same<T, double>::value) {
            value = T(std::stod(opt, nullptr));
        } else {
            value = T(std::stoull(opt, nullptr, 0));
        }
    } catch (std::invalid_argument& ) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range& ) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

/**
 * @brief Get a set of values from the option. The option can be
 *        a number, a range or alist of numbers and ranges. A number
 *        can be `0` or `1`. A range is numbers separate by `-` and
 *        a list is delimited by `,`.
 *        For example:
 *           100
 *           4-7
 *           100,4-7,109-200
 * @tparam T The type of values to return
 * @param[in] opt The option to extract the values from
 * @param[in] max_value The maximum value that can be return. If 0 no maximum.
 *                      Defaults to 0
 * @param[in] no_error If true no error is raise. Defaults to false.
 * @returns A vector of type T of values from the option
 */
template<typename T>
static std::vector<T> get_values(
    const std::string& opt, const size_t max_value = 0, bool no_error = false) {
    std::vector<T> values;
    if (opt == "all") {
        if (max_value == 0) {
            throw std::runtime_error("range `all` invalid, max count is unknown");
        }
        values.resize(max_value);
        std::iota(values.begin(), values.end(), 0);
    } else {
        string::strings sc;
        string::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::string::strings sd;
            xia::util::string::split(sd, slots, '-');
            if (sd.size() == 1) {
                if (sd[0] == "all") {
                    for (T s = T(0); s < T(max_value); ++s) {
                        values.push_back(s);
                    }
                } else {
                    T val = get_value<T>(sd[0]);
                    if (max_value == 0 || val < T(max_value)) {
                        values.push_back(val);
                    } else if (!no_error) {
                        throw std::runtime_error(
                            "value out of range: " + std::to_string(val));
                    }
                }
            } else if (sd.size() == 2) {
                size_t start = get_value<T>(sd[0]);
                size_t end = get_value<T>(sd[1]);
                if (start > end) {
                    if (!no_error) {
                        throw std::runtime_error("invalid range: " + opt);
                    }
                    values.clear();
                    break;
                }
                if (max_value == 0 ||
                    (start < max_value && end < max_value)) {
                    for (T s = T(start); s <= T(end); ++s) {
                        values.push_back(s);
                    }
                } else if (!no_error) {
                    auto val = start < max_value ? end : start;
                    throw std::runtime_error(
                        "value out of range: " + std::to_string(val));
                }
            } else {
                if (!no_error) {
                    throw std::runtime_error("invalid range: " + opt);
                }
                values.clear();
                break;
            }
        }
    }
    return values;
}

/**
 * @brief Output a name/value pair and if not a double output in hex
 * @tparam V The type of value to output
 * @param[in,out] out The stream to putput the value to.
 * @param[in] name The name of the value.
 * @param[in] value The number that we'll output.
 * @param[in] eol If true output and end of line character, Default if true.
 */
template<typename V>
static void output_value(
    std::ostream& out, const std::string& name, V value, bool eol = true) {
    util::io::ostream_guard oguard(out);
    out << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        out << std::hex << " (0x" << value << ')';
    }
    if (eol) {
        out << std::endl;
    }
}

/**
 * @brief Humanize a number into standard size names. Ex. gigabyte (GB).
 * @tparam T A type that can be streamed into a ostringstream
 * @param[in] value The number that we'll humanize.
 * @param[in] suffix Any additional information to go after the unit.
 * @returns The string as a human-readable number
 */
template<typename T>
std::string humanize(T value, const std::string suffix = "") {
    const char* units = " KMGTPEZY";
    const char* unit = units;
    double num = static_cast<double>(value);
    while (*unit != '\0') {
        if (std::abs(num) < 1024.0) {
            break;
        }
        ++unit;
        num /= 1024.0;
    }
    if (*unit == '\0') {
        --unit;
    }
    std::ostringstream oss;
    oss << std::setprecision(3) << std::fixed << num << *unit << suffix;
    return oss.str();
}
} // namespace io
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_IO_HPP
