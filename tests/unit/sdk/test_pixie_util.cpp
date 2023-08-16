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

/** @file test_pixie_util.cpp
 * @brief Provides test coverage for SDK utility functions
 */


#include <doctest/doctest.h>
#include <pixie/utils/crc.hpp>
#include <pixie/utils/io.hpp>
#include <pixie/utils/numerics.hpp>
#include <pixie/utils/path.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/utils/time.hpp>

TEST_SUITE("xia::util") {
    TEST_CASE("Verify ostream_guard") {
        std::ios_base::fmtflags expected(std::cout.flags());
        xia::util::io::ostream_guard ostream_guard(std::cout);
        CHECK(ostream_guard.flags == expected);
    }

    TEST_CASE("ieee_float") {
        SUBCASE("Initialization") {
            CHECK(xia::util::numerics::ieee_float(0.5) == 0.5);
            CHECK(xia::util::numerics::ieee_float(xia::util::numerics::ieee_float(0.5)) == 0.5);
            CHECK(xia::util::numerics::ieee_float(0x3f000000u) == 0.5);
        }
        SUBCASE("Equality Operator") {
            CHECK(xia::util::numerics::ieee_float(0.5) == xia::util::numerics::ieee_float(0.5));
        }
        SUBCASE("Cast operator / out()") {
            CHECK(double(xia::util::numerics::ieee_float(0.5)) == 0.5);
            CHECK(double(xia::util::numerics::ieee_float(-0.5)) == -0.5);
        }
        SUBCASE("Sign Bit 0 / Exponent > 0") {
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(0x40490fdbu)) == 3.14159);
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(3.14159)) ==
                  xia::util::numerics::ieee_float(0x40490fdbu));
        }
        SUBCASE("Sign Bit 1 / Exponent > 0") {
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(0xc958a450u)) == -887365);
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(-887365.)) ==
                  xia::util::numerics::ieee_float(0xc958a450u));
        }
        SUBCASE("Sign Bit 0 / Exponent < 0") {
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(0x3e22d0e5u)) == 0.159);
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(0.159)) ==
                  xia::util::numerics::ieee_float(0x3e22d0e5u));
        }
        SUBCASE("Sign Bit 1 / Exponent < 0") {
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(0xbe22d0e5u)) == -0.159);
            CHECK(doctest::Approx(xia::util::numerics::ieee_float(-0.159)) ==
                  xia::util::numerics::ieee_float(0xbe22d0e5u));
        }
        SUBCASE("Sign Bit 0 / Exponent = 0") {
            CHECK(xia::util::numerics::ieee_float(0x3f800000u) == 1.0);
            CHECK(xia::util::numerics::ieee_float(1.0) == xia::util::numerics::ieee_float(0x3f800000u));
        }
        SUBCASE("Sign Bit 1 / Exponent = 0") {
            CHECK(xia::util::numerics::ieee_float(0xbf800000u) == -1.0);
            CHECK(xia::util::numerics::ieee_float(-1.0) == xia::util::numerics::ieee_float(0xbf800000u));
        }
    }

    TEST_CASE("dequote") {
        std::string good = "\"quoted\"";
        xia::util::string::dequote(good);
        CHECK(good == "quoted");

        std::string bad = "\"quoted\'";
        CHECK_THROWS_WITH_AS(xia::util::string::dequote(bad), "invalid quoting: \"quoted\'",
                             std::runtime_error);
    }

    TEST_CASE("ltrim") {
        std::string test = "    trim";
        xia::util::string::ltrim(test);
        CHECK(test == "trim");
    }

    TEST_CASE("rtrim") {
        std::string test = "trim    ";
        xia::util::string::rtrim(test);
        CHECK(test == "trim");
    }

    TEST_CASE("split") {
        std::string test = "a,b,c,d";
        xia::util::string::strings result;
        SUBCASE("Basic split") {
            xia::util::string::split(result, test, ',', 0, false, false, false);
            CHECK(result == xia::util::string::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with limit") {
            xia::util::string::split(result, test, ',', 2, false, false, true);
            CHECK(result == xia::util::string::strings{"a", "b"});
        }
        SUBCASE("Split with spaces") {
            std::string test_w_spaces = "    a,b   ,c,d";
            xia::util::string::split(result, test_w_spaces, ',', 0, true, false, true);
            CHECK(result == xia::util::string::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with quotes") {
            std::string test_w_quotes = "\"a\",b,\'c\',d";
            xia::util::string::split(result, test_w_quotes, ',', 0, false, true, true);
            CHECK(result == xia::util::string::strings{"a", "b", "c", "d"});
        }
        SUBCASE("Split with bad quotes") {
            std::string test_w_quotes = "\"a\",b,\'c\",d";
            CHECK_THROWS_WITH_AS(xia::util::string::split(result, test_w_quotes, ',', 0, false, true, true),
                                 "invalid quoting: \'c\"", std::runtime_error);
        }
        SUBCASE("Split with quotes and spaces - in element") {
            result.clear();
            std::string test_w_quotes_and_space = "\"  a\",  b,\'c  \',  d";
            xia::util::string::split(result, test_w_quotes_and_space, ',', 0, true, true, true);
            CHECK(result == xia::util::string::strings{"  a", "b", "c  ", "d"});
        }
        SUBCASE("Split with quotes and spaces - around element") {
            result.clear();
            std::string test_w_quotes_and_space_outside = "  \"a\",  b  ,\'c  \'  ,  d";
            xia::util::string::split(result, test_w_quotes_and_space_outside, ',', 0, true, true, true);
            CHECK(result == xia::util::string::strings{"a", "b", "c  ", "d"});
        }
    }

    TEST_CASE("crc32") {
        std::vector<unsigned char> vec_val = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
        const uint32_t expected = 0xcbf43926;
        const std::string expected_str = "cbf43926";

        SUBCASE("Update with a vector") {
            auto chksum1 = xia::util::crc::crc32();
            chksum1.update(vec_val);
            CHECK(chksum1.value == expected);
            CHECK(static_cast<std::string>(chksum1) == expected_str);
        }
        SUBCASE("Update with a stream") {
            auto chksum2 = xia::util::crc::crc32();
            for (const auto val : vec_val)
                chksum2 << val;
            CHECK(chksum2.value == expected);
        }
        SUBCASE("Clear") {
            auto chksum3 = xia::util::crc::crc32();
            chksum3.value = expected;
            chksum3.clear();
            CHECK(chksum3.value == 0);
        }
    }

    TEST_CASE("ISO-8601 Timestamps") {
        xia::util::time::datetime_timepoint test_time{std::chrono::milliseconds{794765775123}};
        std::string test_time_str = xia::util::time::datetime_iso8601(test_time);
        SUBCASE("Timestamp as string") {
            CHECK(test_time_str == "1995-03-09T16:16:15.123Z");
        }
        SUBCASE("Timestamp as file name") {
            xia::util::time::datetime_iso8601_as_filename(test_time_str);
            CHECK(test_time_str == "1995-03-09T16-16-15.123Z");
        }
    }

    TEST_CASE("extension") {
        std::string test_good = "testfile.bin";
        std::string test_bad = "testfile";
        std::string test_end = "testfile.";
        std::string ext_good = xia::util::path::extension(test_good);
        std::string ext_bad = xia::util::path::extension(test_bad);
        std::string ext_end = xia::util::path::extension(test_end);
        CHECK(ext_good == "bin");
        CHECK(ext_bad == "testfile");
        CHECK(ext_bad == "testfile");
    }
}
