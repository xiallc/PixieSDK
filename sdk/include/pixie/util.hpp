#ifndef PIXIE16_UTIL_H
#define PIXIE16_UTIL_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace xia
{
namespace util
{
/*
 * String trim and split support.
 */
typedef std::vector<std::string> strings;

/**
 * Split a string into a vector of strings.
 *
 * @param split_string The split string
 * @param s String to split
 * @param count Number of split to make before returning
 * @param strip_whitespace Trim white space from the split strings
 * @param strip_quotes Remove quotes from string fragments
 * @param empty Include empty string fragments
 */
void split(strings& split_string,
           const std::string& s,
           const char delimiter = ' ',
           size_t count = 0,
           bool strip_whitespace = true,
           bool strip_quotes = true,
           bool empty = false);

/*
 * Remove spaces from the front of the string.
 */
void ltrim(std::string& s);

/*
 * Remove spaces from the end of the string
 */
void rtrim(std::string& s);

/*
 * Trim spaces from both ends of a string
 */
void trim(std::string& s);

/*
 * Dequote a string
 */
void dequote(std::string& s);

/*
 * Humanize a number
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
    oss << std::setprecision(3) << std::fixed
        << num << *unit << suffix;
    return oss.str();
}

/*
 * Save and restore the output stream's settings.
 */
struct ostream_guard {
    std::ostream& o;
    std::ios_base::fmtflags flags;
    ostream_guard(std::ostream& o_)
      : o(o_),
        flags(o_.flags()) {
    }
    ~ostream_guard() {
        o.flags(flags);
    }
};

/*
 * Timepiont measures a period of time between two points.
 */
struct timepoint {
    using marker = std::chrono::time_point<std::chrono::steady_clock>;

    std::atomic_bool active;
    std::atomic_bool suspended;
    std::atomic_bool captured;

    marker start_mark;
    marker end_mark;

    timepoint(bool autostart = false);
    ~timepoint();

    bool running() const {
        return active.load() && !suspended.load();
    }

    void reset();
    void start();
    void end();
    void stop() { end(); }
    void restart();
    void pause();
    void resume();

    uint64_t secs();
    uint64_t msecs();
    uint64_t usecs();

    operator std::string ();
    std::string output();

private:

    void lock();
    void unlock();

    std::atomic_bool locked;
};

/*
 * IEEE float as a type.
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

    operator double() const;
    operator value_type() const;

private:
    value_type in(const double dec_num) const;
    double out() const;

    value_type value;
};

/*
 * CRC32 checksum
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * The polynomial is
 * `X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0`
 */
struct crc32 {
    using value_type = uint32_t;

    value_type value;

    template <typename T> void update(const T val) {
        update(static_cast<const unsigned char*>(&val), sizeof(T));
    }

    template <typename T> void update(const std::vector<T>& vals,
                                      const size_t start = 0) {
        const size_t so = ((vals.size() - start) *
                           sizeof(typename std::vector<T>::value_type));
        update(static_cast<const unsigned char*>(&vals[start]), so);
    }

    template <typename T> crc32& operator<<(const T& val) {
        update(val);
        return *this;
    }

    crc32& operator<<(const std::string& val);

    operator const std::string() const;

    crc32();

    void clear();

private:
    void update(const unsigned char* data, int len);
    /// A precomputed lookup table for the CRC32 bitwise algorithm
    static const value_type table[256];
};

}
}

/*
 * Output stream operator.
 */
std::ostream&
operator<<(std::ostream& out, xia::util::timepoint& timepoint);

#endif  // PIXIE16_UTIL_H
