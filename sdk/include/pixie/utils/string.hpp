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

#include <functional>
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
void split(strings& split_string, const char* s, const char delimiter = ' ',
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
        if (!ns.empty()) {
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
 * @brief Replace a character in a string with another character
 * @param[in,out] s the string that we'll replace characters in.
 * @param[in] find the character to locate in the string and replace
 * @param[in] replace the character to replace any foind characters
 */
void replace(std::string& target, const char find, const char replace);

/**
 * @brief Return true if a string starts with another string
 * @param[in] s the string we string check to see if it starts with a string
 * @param[in] check the string we check is at the start
 * @return true if the string begins with the check string
 */
bool starts_with(const std::string& s, const std::string& check);

/**
 * @brief Convert a string to lower case
 * @param[in,out] s the string we make lower case
 */
void tolower(std::string& s);

/**
 * @brief Convert a string to upper case
 * @param[in,out] s the string we make upper case
 */
void toupper(std::string& s);

/**
 * @brief Dequote a string
 * @param[in,out] s the string that we'll trim.
 */
void dequote(std::string& s);

/**
 * @brief Check if a string is a number
 * @param[in] s the string that we'll check.
 * @return @true if the string is a number else @false
 */
bool check_number(const std::string& s);

/**
 * @brief Check if a string is a number or number range
 * @param[in] s the string that we'll check.
 * @return @true if the string is a number or number range else @false
 */
bool check_number_range(const std::string& s);

/**
 * @brief Check if a string is an affirmative value.
 * @param[in] s the string that we'll check.
 * @return @true if the string is affirmative else @false
 */
bool check_affirmative(const std::string& s);

/**
 * @brief Token editor splits a string by a token separator allowing
 * you to add, edit or remove tokens. You can export the list as a
 * string. This is useful for holding state information in MIB
 * strings. Removing and updating uses a regular expression string to
 * find the token.
 */
struct token_editor {
    using edit_func = std::function<bool(std::string& token)>;

    strings tokens;
    char separator;
    bool sorted;

    token_editor(const std::string& str, char separator =',', bool sort = true);
    token_editor(const char* str, char separator =',', bool sort = true);
    token_editor();
    token_editor(const token_editor& orig);

    void set(const std::string& str);
    void set(const char* str);

    std::string get() const;
    std::string get(const std::string& token_regx) const;
    std::string get(const char* token_regx) const;

    bool has(const std::string& token) const;
    bool has(const char* token) const;

    void add(const std::string& token);
    void add(const char* token);

    void remove(const std::string& token_regx);
    void remove(const char* token_regx);

    void update(const std::string& token_regx, const std::string& token);
    void update(const char* token_regx, const char* token);
    void update(const std::string& token_regx, edit_func editor);
    void update(const char* token_regx, edit_func editor);

    void sort();

    token_editor& operator=(const std::string& str);
    token_editor& operator=(const char* str);
};
} // namespace string
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_STRING_HPP
