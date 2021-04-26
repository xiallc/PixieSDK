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

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include <pixie_util.hpp>

namespace xia
{
namespace util
{
void
dequote (std::string& s)
{
    if (!s.empty())
    {
      char front = s[0];
      char back = s[s.length () - 1];
      if ((front == '"') || (front == '\''))
      {
        if (front != back)
            throw std::runtime_error("invalid quoting: " + s);
        s =  s.substr(1, s.length() - (1 + 1));
      }
    }
  }

void
split(strings& split_string,
      const std::string& s,
      const char delimiter,
      size_t count,
      bool strip_whitespace,
      bool strip_quotes,
      bool empty)
{
    std::stringstream ss(s);
    std::string       e;
    split_string.clear();
    while (std::getline(ss, e, delimiter))
    {
        if (strip_whitespace)
            trim(e);
        if (strip_quotes)
            dequote(e);
        if (empty || !e.empty())
        {
            split_string.push_back(e);
            if (count != 0 && split_string.size() >= count) {
                break;
            }
        }
    }
}

void
ltrim(std::string& s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch) {
                             return !std::isspace(ch); }));
}

void
rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); }).base(),
            s.end());
}

void
trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

ieee_float::ieee_float()
    : value(0)
{
}

ieee_float::ieee_float(const ieee_float& ieee)
    : value(ieee.value)
{
}

ieee_float::ieee_float(value_type value_)
    : value(value_)
{
}

ieee_float::ieee_float(const double dec_num)
    : value(in(dec_num))
{
}

ieee_float&
ieee_float::operator=(const ieee_float& ieee)
{
    value = ieee.value;
    return *this;
}

ieee_float&
ieee_float::operator=(const double dec_num)
{
    value = in(dec_num);
    return *this;
}

bool
ieee_float::operator==(const ieee_float& ieee) const
{
    return value == ieee.value;
}

bool
ieee_float::operator==(const double dec_num) const
{
    return value == in(dec_num);
}

ieee_float::operator double() const
{
    return out();
}

ieee_float::value_type
ieee_float::in(const double dec_num) const
{
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
        value_type exponent = 127 + k - 1;

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
        for (n = 31; n >0; --n) {
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

double
ieee_float::out() const
{
    short signbit = static_cast<short>(value >> 31);
    short exponent = static_cast<short>(((value & 0x7F800000) >> 23) - 127);
    double mantissa = 1.0 + ((double(value & 0x7FFFFF) / double(1 << 23)));
    double result;
    if (signbit == 0) {
        result = mantissa * (double(1 << exponent));
    } else {
        result = -(mantissa * (double(1 << exponent)));
    }
    return result;
}

}
}
