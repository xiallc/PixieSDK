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
#include <sstream>
#include <string>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace io {

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

} // namespace io
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_IO_HPP
