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
/// @file test_pixie_util.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date April 20, 2021

#include "doctest.h"
#include "pixie_util.hpp"

TEST_SUITE("xia::util") {
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
        SUBCASE("Equality Operator") {
            CHECK(xia::util::ieee_float(0.5) == xia::util::ieee_float(0.5));
        }
        SUBCASE("Cast operator / out()") {
            CHECK(static_cast<double>(xia::util::ieee_float(0.5)) == 0.5);
            CHECK(static_cast<double>(xia::util::ieee_float(-0.5)) == -0.5);
        }
        SUBCASE("Sign Bit 0 / Exponent > 0") {
            CHECK(doctest::Approx(xia::util::ieee_float(0x40490fdbu)) == 3.14159);
            CHECK(doctest::Approx(xia::util::ieee_float(3.14159)) ==
                  xia::util::ieee_float(0x40490fdbu));
        }
        SUBCASE("Sign Bit 1 / Exponent > 0") {
            CHECK(doctest::Approx(xia::util::ieee_float(0xc958a450u)) == -887365);
            CHECK(doctest::Approx(xia::util::ieee_float(-887365.)) ==
                  xia::util::ieee_float(0xc958a450u));
        }
        SUBCASE("Sign Bit 0 / Exponent < 0") {
            CHECK(doctest::Approx(xia::util::ieee_float(0x3e22d0e5u)) == 0.159);
            CHECK(doctest::Approx(xia::util::ieee_float(0.159)) ==
                  xia::util::ieee_float(0x3e22d0e5u));
        }
        SUBCASE("Sign Bit 1 / Exponent < 0") {
            CHECK(doctest::Approx(xia::util::ieee_float(0xbe22d0e5u)) == -0.159);
            CHECK(doctest::Approx(xia::util::ieee_float(-0.159)) ==
                  xia::util::ieee_float(0xbe22d0e5u));
        }
        SUBCASE("Sign Bit 0 / Exponent = 0") {
            CHECK(xia::util::ieee_float(0x3f800000u) == 1.0);
            CHECK(xia::util::ieee_float(1.0) == xia::util::ieee_float(0x3f800000u));
        }
        SUBCASE("Sign Bit 1 / Exponent = 0") {
            CHECK(xia::util::ieee_float(0xbf800000u) == -1.0);
            CHECK(xia::util::ieee_float(-1.0) == xia::util::ieee_float(0xbf800000u));
        }
    }

    TEST_CASE("dequote") {
        std::string good = "\"quoted\"";
        xia::util::dequote(good);
        CHECK(good == "quoted");

        std::string bad = "\"quoted\'";
        CHECK_THROWS_WITH_AS(xia::util::dequote(bad), "invalid quoting: \"quoted\'",
                             std::runtime_error);
    }

    TEST_CASE("ltrim") {
        std::string test = "    trim";
        xia::util::ltrim(test);
        CHECK(test == "trim");
    }

    TEST_CASE("rtrim") {
        std::string test = "trim    ";
        xia::util::rtrim(test);
        CHECK(test == "trim");
    }

    TEST_CASE("split") {
        std::string test = "a,b,c,d";
        xia::util::strings result;
        SUBCASE("Basic split") {
            xia::util::split(result, test, ',', 0, false, false, false);
            CHECK(result == xia::util::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with limit") {
            xia::util::split(result, test, ',', 2, false, false, true);
            CHECK(result == xia::util::strings{"a", "b"});
        }
        SUBCASE("Split with spaces") {
            std::string test_w_spaces = "    a,b   ,c,d";
            xia::util::split(result, test_w_spaces, ',', 0, true, false, true);
            CHECK(result == xia::util::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with quotes") {
            std::string test_w_quotes = "\"a\",b,\'c\',d";
            xia::util::split(result, test_w_quotes, ',', 0, false, true, true);
            CHECK(result == xia::util::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with bad quotes") {
            std::string test_w_quotes = "\"a\",b,\'c\",d";
            CHECK_THROWS_WITH_AS(xia::util::split(result, test_w_quotes, ',', 0, false, true, true),
                                 "invalid quoting: \'c\"", std::runtime_error);
        }
        SUBCASE("Split with quotes and spaces - in element") {
            result.clear();
            std::string test_w_quotes_and_space = "\"  a\",  b,\'c  \',  d";
            xia::util::split(result, test_w_quotes_and_space, ',', 0, true, true, true);
            CHECK(result == xia::util::strings{"  a", "b", "c  ", "d"});
        }
        SUBCASE("Split with quotes and spaces - around element") {
            result.clear();
            std::string test_w_quotes_and_space_outside = "  \"a\",  b  ,\'c  \'  ,  d";
            xia::util::split(result, test_w_quotes_and_space_outside, ',', 0, true, true, true);
            CHECK(result == xia::util::strings{"a", "b", "c  ", "d"});
        }
    }
}
