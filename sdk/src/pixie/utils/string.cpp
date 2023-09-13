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

/** @file string.cpp
 * @brief Defines utility functions and data structures related to strings useful throughout the SDK.
 */

#include <algorithm>
#include <regex>

#include <pixie/error.hpp>
#include <pixie/utils/string.hpp>

namespace xia {
namespace util {
namespace string {
void split(strings& split_string, const char* s, const char delimiter, size_t count,
           bool strip_whitespace, bool strip_quotes, bool empty) {
    std::stringstream ss(s);
    std::string e;
    split_string.clear();
    while (std::getline(ss, e, delimiter)) {
        if (strip_whitespace)
            trim(e);
        if (strip_quotes)
            dequote(e);
        if (empty || !e.empty()) {
            split_string.push_back(e);
            if (count != 0 && split_string.size() >= count) {
                break;
            }
        }
    }
}

void split(strings& split_string, const std::string& s, const char delimiter, size_t count,
           bool strip_whitespace, bool strip_quotes, bool empty) {
    return split(split_string, s.c_str(), delimiter, count, strip_whitespace, strip_quotes, empty);
}

void ltrim(std::string& s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

void replace(std::string& target, const char find, const char replace) {
    while (true) {
        auto pos = target.find_first_of(find);
        if (pos == std::string::npos) {
            break;
        }
        target.replace(pos, 1, 1, replace);
    }
}

bool starts_with(const std::string& s, const std::string& check) {
    return check.size() <= s.size() && s.compare(0, check.size(), check) == 0;
}

void tolower(std::string& s) {
    std::transform(
        s.cbegin(), s.cend(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

void toupper(std::string& s) {
    std::transform(
        s.cbegin(), s.cend(), s.begin(),
        [](unsigned char c) { return std::toupper(c); });
}

void dequote(std::string& s) {
    if (!s.empty()) {
        char front = s[0];
        char back = s[s.length() - 1];
        if ((front == '"') || (front == '\'')) {
            if (front != back)
                throw std::runtime_error("invalid quoting: " + s);
            s = s.substr(1, s.length() - (1 + 1));
        }
    }
}

bool check_number(const std::string& s) {
    return std::regex_match(
        s, std::regex(("((\\+|-)?[0-9]+)?((\\+|-)?\\.[0-9]+)?([e|E](\\+|-)?[0-9]+)?")));
}

bool check_number_range(const std::string& s) {
    strings sd;
    split(sd, s, '-');
    if (sd.size() == 1) {
        return sd[0] == "all" || check_number(sd[0]);
    } else if (sd.size() == 2) {
        return check_number(sd[0]) && check_number(sd[1]);
    }
    throw std::runtime_error("invalid range: " + s);
}
}
}
}
