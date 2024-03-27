/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

#include <algorithm>
#include <atomic>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <pixie/log.hpp>
#include <pixie/utils/string.hpp>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace io {

using write_string_func = std::function<void(const std::string& out)>;

/**
 * @brief Save and restore the output stream's settings.
 */
struct ostream_guard {
    std::ostream& o;
    std::ios_base::fmtflags flags;
    char fill;
    ostream_guard(std::ostream& o_) : o(o_), flags(o_.flags()), fill(o_.fill()) {}
    ~ostream_guard() {
        o.flags(flags);
        o.fill(fill);
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
 * @brief Get a set of valid values from the option. The option can be
 *        a number, a range or a list of numbers and ranges. A number
 *        can be `0` or `1`. A range is numbers separate by `-` and
 *        a list is delimited by `,`.
 *        For example:
 *           100
 *           4-7
 *           100,4-7,109-200
 * @tparam T The type of values to return
 * @param[out] values The set of valid values from the option.
 * @param[in] opt The option to extract the values from
 * @param[in] valid_values A vector of all possible values that opt can define.
 *                      Defaults to empty vector
 * @returns A vector of type T of values from the option
 */
template<typename T>
static void get_values_in_set(
    std::vector<T>& values, const std::string& opt,
    std::vector<T> valid_values = {}) {
    if (valid_values.empty()) {
        values.clear();
        xia_log(xia::log::warning) << "valid set unknown";
        return;
    }
    if (opt == "all") {
        values = valid_values;
    } else {
        values.clear();
        string::strings sc;
        string::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::string::strings sd;
            xia::util::string::split(sd, slots, '-');
            if (sd.size() == 1) {
                if (sd[0] == "all") {
                    values = valid_values;
                } else {
                    if (std::find(begin(valid_values), end(valid_values), get_value<T>(sd[0]))
                        != valid_values.end()) {
                        values.push_back(get_value<T>(sd[0]));
                    } else {
                        xia_log(xia::log::warning) << "value not in valid set: "
                                                   << get_value<T>(sd[0]);
                    }
                }
            } else if (sd.size() == 2) {
                size_t start = get_value<T>(sd[0]);
                size_t finish = get_value<T>(sd[1]);
                if (start > finish) {
                    xia_log(xia::log::warning) << "invalid range: " << opt;
                    continue;
                }
                for (T s = T(start); s <= T(finish); ++s) {
                    if (std::find(begin(valid_values), end(valid_values), s)
                        != valid_values.end()) {
                        values.push_back(s);
                    } else {
                        xia_log(xia::log::warning) << "value not in valid set: " << s;
                    }
                }
            } else {
                xia_log(xia::log::warning) << "invalid range: " << opt;
            }
        }
    }
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

/**
 * @brief Writes a memory segment as hex values.
 * @see https://git.rtems.org/rtems-tools/tree/rtemstoolkit/rtems-utils.cpp#n39
 * @param[in] addr The address of the memory to display.
 * @param[in] length The number of elements to display.
 * @param[in] writer The function to write the memory segment with.
 * @param[in] label A label that can be used to name the output.
 * @param[in] size The size of the data element.
 * @param[in] line_length Number of elements per line.
 * @param[in] offset The printed offset.
 */
void memdump(const void* addr, size_t length, write_string_func writer, std::string label = "",
             size_t size = 1, size_t line_length = 16, size_t offset = 0);

/**
 * @brief Struct to represent file reading/writing
 */
struct file {
    enum struct flag {
        ro,
        wr,
        rw,
        wr_trunc,
        rw_trunc
    };

    enum struct seek_mode {
        set,
        cur,
        end
    };

    using value_ptr = void*;
    using const_value_ptr = const void*;

    std::atomic_int handle;

    file();
    ~file();

    /**
     * Checks that the file has a valid file handle
     */
    bool valid() {
        return handle >= 0;
    }
    /**
     * Closes the file and resets the file handle.
     */
    void close();
    /**
     * Creates a file with the given filename/path. The file can be created with read-only, write-only,
     * or read/write permissions depending on the flag.
     */
    void create(const std::string& filename, flag flag);
    /**
     * Opens a file with the given filename/path. The file can be opened with read-only, write-only,
     * or read/write permissions depending on the flag.
     */
    void open(const std::string& filename, flag flag);
    /**
     * Performs lseek on the file. Mode determines where the file offset occurs:
     * SEEK_SET = start + offset, SEEK_CUR = current + offset, SEEK_END = end + offset
     */
    template<typename S>
    size_t seek(S offset, seek_mode mode) {
        return seek(static_cast<int64_t>(offset), mode);
    }
    /**
     * Reads a vector of data from the file.
     */
    template<typename D>
    size_t read(std::vector<D>& data) {
        return read(
            reinterpret_cast<value_ptr>(data.data()),static_cast<size_t>(data.size()));
    }
    /**
     * Reads a vector of data from the file.
     */
    template<typename D, typename S>
    size_t read(std::vector<D>& data, S size) {
        return read(
            reinterpret_cast<value_ptr>(data.data()), static_cast<size_t>(size));
    }
    /**
     * Writes a vector of data from the file.
     */
    template<typename D>
    size_t write(const std::vector<D>& data) {
        return write(
            static_cast<const_value_ptr>(data.data()), static_cast<size_t>(data.size()));
    }
    /**
     * Writes a vector of data from the file.
     */
    template<typename D, typename S>
    size_t write(const std::vector<D>& data, S size) {
        return write(
            static_cast<const_value_ptr>(data.data()), static_cast<size_t>(size));
    }

private:
    /*
     * Declared here to resolve their respective template functions.
     */
    int64_t seek(int64_t offset, seek_mode mode);
    size_t read(value_ptr data, size_t size);
    size_t write(const_value_ptr data, size_t size);
};

} // namespace io
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_IO_HPP
