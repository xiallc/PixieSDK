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
    typedef std::chrono::time_point<std::chrono::steady_clock> marker;

    bool active;
    bool suspended;
    bool captured;
    marker start_mark;
    marker end_mark;

    timepoint(bool autostart = false);
    ~timepoint();

    void reset();
    void start();
    void end();
    void restart();
    void pause();
    void resume();

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
    typedef unsigned int value_type;

    ieee_float();
    ieee_float(const ieee_float& ieee);
    ieee_float(value_type value_);
    ieee_float(double dec_num);

    ieee_float& operator=(const ieee_float& ieee);
    ieee_float& operator=(const double dec_num);

    bool operator==(const ieee_float& ieee) const;
    bool operator==(const double dec_num) const;

    operator double() const;

private:
    value_type in(const double dec_num) const;
    double out() const;

    value_type value;
};
}
}

/*
 * Output stream operator.
 */
std::ostream&
operator<<(std::ostream& out, xia::util::timepoint& timepoint);

#endif  // PIXIE16_UTIL_H
