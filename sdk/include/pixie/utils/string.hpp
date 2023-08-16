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

/** @file string.hpp
 * @brief Defines utility functions and data structures related to strings useful throughout the SDK.
 */

#ifndef PIXIESDK_UTIL_STRING_HPP
#define PIXIESDK_UTIL_STRING_HPP

#include <string>
#include <vector>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace string {
/**
 * @brief Defines a type for a vector of strings.
 */
using strings = std::vector<std::string>;

/**
 * @brief Split a string into a vector of strings.
 * @param[out] split_string The split string
 * @param[in] s String to split
 * @param[in] delimiter delimiter to split on. Default: ' '.
 * @param[in] count Number of split to make before returning. Default: 0.
 * @param[in] strip_whitespace Trim white space from the split strings. Default: True
 * @param[in] strip_quotes Remove quotes from string fragments. Default: True
 * @param[in] empty Include empty string fragments. Default: False.
 */
void split(strings& split_string, const std::string& s, const char delimiter = ' ',
           size_t count = 0, bool strip_whitespace = true, bool strip_quotes = true,
           bool empty = false);

/**
 * @brief Join the strings into a single string.
 * @tparam T Templated parameter for the stream type
 * @param ss stream containing the data to join.
 * @param c delimiter to join with
 * @return The joined string
 */
template<typename T>
std::string join(T& ss, const char c = ' ') {
    std::string ns;
    for (auto& s : ss) {
        if (!ss.empty()) {
            ns += c;
        }
        ns += s;
    }
    return ns;
}

/**
 * @brief Remove spaces from the front of the string.
 * @param[in,out] s the string that we'll trim.
 */
void ltrim(std::string& s);

/**
 * @brief Remove spaces from the end of the string
 * @param[in,out] s the string that we'll trim.
 */
void rtrim(std::string& s);

/**
 * @brief Trim spaces from both ends of a string
 * @param[in,out] s the string that we'll trim.
 */
void trim(std::string& s);

/**
 * @brief Dequote a string
 * @param[in,out] s the string that we'll trim.
 */
void dequote(std::string& s);

} // namespace string
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_STRING_HPP
