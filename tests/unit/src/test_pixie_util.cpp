/**----------------------------------------------------------------------
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
*----------------------------------------------------------------------**/
/// @file test_pixie_log.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date April 15, 2021

#include "doctest.h"
#include "pixie_util.hpp"

TEST_SUITE("xia::pixie") {
    TEST_CASE("Verify ostream_guard") {
        std::ios_base::fmtflags expected(std::cout.flags());
        xia::util::ostream_guard ostream_guard(std::cout);
        CHECK(ostream_guard.flags == expected);
    }
    TEST_CASE("ieee_float") {
        SUBCASE("Initialization") {
            CHECK(xia::util::ieee_float(0.5) == 0.5);
            CHECK(xia::util::ieee_float(xia::util::ieee_float(0.5)) == 0.5);
            CHECK(xia::util::ieee_float(0x3f000000u) == 0.5);
        }
        SUBCASE("Operators") {
            xia::util::ieee_float ieee = 0.5;
            xia::util::ieee_float copy = ieee;
            CHECK(ieee == copy);
            CHECK(ieee == xia::util::ieee_float(0.5));
            CHECK(static_cast<double>(ieee) == 0.5);
        }
        SUBCASE("Conversions") {
            CHECK(xia::util::ieee_float(0x3f800000u) == 1.0);
            CHECK(xia::util::ieee_float(0xbf800000u) == -1.0);
            CHECK(xia::util::ieee_float(0x40490fdbu) == 3.14159);
            CHECK(xia::util::ieee_float(0x4958a450u) == 887635.0);
            CHECK(xia::util::ieee_float(1.0) == xia::util::ieee_float(0x3f800000u));
            CHECK(xia::util::ieee_float(-1.0) == xia::util::ieee_float(0xbf800000u));
            CHECK(xia::util::ieee_float(3.14159) == xia::util::ieee_float(0x40490fdbu));
            CHECK(xia::util::ieee_float(88763.0) == xia::util::ieee_float(0x4958a450u));
        }
    }
}
