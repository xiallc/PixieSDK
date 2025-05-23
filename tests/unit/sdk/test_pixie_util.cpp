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

#include <algorithm>

#include <doctest/doctest.h>
#include <pixie/os_compat.h>
#include <pixie/utils/crc.hpp>
#include <pixie/utils/io.hpp>
#include <pixie/utils/numerics.hpp>
#include <pixie/utils/path.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/utils/thread.hpp>
#include <pixie/utils/time.hpp>
#include <pixie/utils/version.hpp>


/*
 * RTEMS file system is empty. We need to create a file to test with.
 */
#if defined(__rtems__)
#define CREATE_TEST_FILE 1
#endif /* __rtems__ */

#if CREATE_TEST_FILE
#define PATH_TEST_FILE "test/sdk/test_pixie_util.cpp"
#include <fstream>
#include <sys/stat.h>
#else
#define PATH_TEST_FILE __FILE__
#endif /* CREATE_TEST_FILE */

static void create_test_file() {
#if CREATE_TEST_FILE
    mkdir("test", S_IRWXU | S_IRWXG | S_IRWXO);
    mkdir("test/sdk", S_IRWXU | S_IRWXG | S_IRWXO);
    std::ofstream out(PATH_TEST_FILE, std::ios::out | std::ios::binary);
    out << "hello" << std::endl;
    out.close();
#endif
}

TEST_SUITE("xia::util") {
    TEST_CASE("Verify ostream_guard") {
        auto expected_flags(std::cout.flags());
        auto expected_fill = std::cout.fill();
        {
            xia::util::io::ostream_guard ostream_guard(std::cout);
            std::cout << std::hex << std::setfill('0');
            CHECK(ostream_guard.flags == expected_flags);
            CHECK(ostream_guard.fill == expected_fill);
            CHECK(std::cout.flags() != expected_flags);
            CHECK(std::cout.fill() != expected_fill);
        }
        CHECK(std::cout.flags() == expected_flags);
        CHECK(std::cout.fill() == expected_fill);
    }

    TEST_CASE("Getting values from options") {
        SUBCASE("Get value") {
            CHECK(xia::util::io::get_value<size_t>("100") == size_t(100));
            CHECK(xia::util::io::get_value<size_t>("0100") == size_t(64));
            CHECK(xia::util::io::get_value<size_t>("0x100") == size_t(256));
            CHECK(xia::util::io::get_value<size_t>("-100") == size_t(-100));
            CHECK(xia::util::io::get_value<int>("12345") == 12345);
            CHECK(xia::util::io::get_value<int>("-12345") == -12345);
            CHECK(xia::util::io::get_value<double>("1.2345") == double(1.2345));
            CHECK(xia::util::io::get_value<double>("-1.2345e9") == double(-1.2345e9));
            CHECK_THROWS_WITH_AS(
                xia::util::io::get_value<int>("one-two-thread"),
                "invalid number: one-two-thread", std::runtime_error);
        }
        SUBCASE("Get values") {
            auto vi = xia::util::io::get_values<int>("0,1,2,3,4-8,9,10-19");
            CHECK(vi.size() == 20);
            for (int i = 0; i < int(vi.size()); ++i) {
                CHECK(vi[i] == i);
            }
            CHECK_THROWS_WITH_AS(
                xia::util::io::get_values<int>("2-1"),
                "invalid range: 2-1", std::runtime_error);
            vi = xia::util::io::get_values<int>("0-19", 20);
            CHECK(vi.size() == 20);
            for (int i = 0; i < int(vi.size()); ++i) {
                CHECK(vi[i] == i);
            }
            CHECK_THROWS_WITH_AS(
                xia::util::io::get_values<int>("0-19", 10);,
                "value out of range: 19", std::runtime_error);
            vi = xia::util::io::get_values<int>("0-19", 10, true);
            CHECK(vi.size() == 0);
            vi = xia::util::io::get_values<int>("0-19,9", 10, true);
            CHECK(vi.size() == 1);
            CHECK(vi[0] == 9);
            vi = xia::util::io::get_values<int>("all", 10);
            CHECK(vi.size() == 10);
            for (int i = 0; i < int(vi.size()); ++i) {
                CHECK(vi[i] == i);
            }
            vi = xia::util::io::get_values<int>("all,0-9", 10);
            CHECK(vi.size() == 20);
            for (int i = 0; i < 10; ++i) {
                CHECK(vi[i] == i);
                CHECK(vi[i + 10] == i);
            }
        }
    }

    TEST_CASE("Getting values in set from options") {
        std::vector<size_t> test_set = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<size_t> ret_set;
        xia::util::io::get_values_in_set<size_t>(ret_set, "1-10", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "1,2,3,4,5,6,7,8,9,10", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "all", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "1-5,6-10", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "1-4,5,6-10", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        test_set = {1, 2, 3, 4, 7, 8, 9, 10};
        ret_set = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        xia::util::io::get_values_in_set<size_t>(ret_set, "1-4,7-10", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "all", test_set);
        CHECK(ret_set == test_set);
        ret_set.clear();
        CHECK_NOTHROW(xia::util::io::get_values_in_set<size_t>(ret_set, "1-10", test_set));
        CHECK(ret_set == test_set);
        ret_set.clear();
        xia::util::io::get_values_in_set<size_t>(ret_set, "1-4", test_set);
        for (size_t i = 1; i <= 4; i++) {
            CHECK(ret_set[i-1] == i);
        }
        ret_set.clear();
        CHECK_NOTHROW(xia::util::io::get_values_in_set<size_t>(ret_set, "2-1", test_set));
        CHECK(ret_set.empty());
        ret_set.clear();
        CHECK_NOTHROW(xia::util::io::get_values_in_set<size_t>(ret_set, "1-4,5,7-10", test_set));
        CHECK(ret_set == test_set);
        ret_set.clear();
        CHECK_NOTHROW(xia::util::io::get_values_in_set<size_t>(ret_set, "1-4,2-1,7-10", test_set));
        CHECK(ret_set == test_set);
        test_set.clear();
        CHECK_NOTHROW(xia::util::io::get_values_in_set<size_t>(ret_set, "1-10", test_set));
        CHECK(ret_set.empty());
    }

    TEST_CASE("Output value") {
        std::ostringstream o1;
        xia::util::io::output_value(o1, "name", 1, false);
        CHECK(o1.str() == "name = 1 (0x1)");
        std::ostringstream o2;
        xia::util::io::output_value(o2, "name", 0100, false);
        CHECK(o2.str() == "name = 64 (0x40)");
        std::ostringstream o3;
        xia::util::io::output_value(o3, "name", 1.2345, false);
        CHECK(o3.str() == "name = 1.2345");
    }

    TEST_CASE("Humanize") {
        CHECK(xia::util::io::humanize(100) == "100.000 ");
        CHECK(xia::util::io::humanize(1024) == "1.000K");
        CHECK(xia::util::io::humanize(1234) == "1.205K");
        CHECK(xia::util::io::humanize(1234 * 1024) == "1.205M");
        CHECK(xia::util::io::humanize(1234 * 1024 * 1024, "ABC") == "1.205GABC");
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

    TEST_CASE("replace") {
        std::string test = "1-2-3-4-5-6";
        xia::util::string::replace(test, '-', '.');
        CHECK(test == "1.2.3.4.5.6");
    }

    TEST_CASE("starts with") {
        CHECK(xia::util::string::starts_with("check starts with", "check") == true);
        CHECK(xia::util::string::starts_with("check starts with", "check starts with") == true);
        CHECK(xia::util::string::starts_with("check starts with", "check starts with this") == false);
        CHECK(xia::util::string::starts_with("check starts with", "do not check") == false);
    }

    TEST_CASE("tolower") {
        std::string test = "Convert To LOWERcase";
        xia::util::string::tolower(test);
        CHECK(test == "convert to lowercase");
    }

    TEST_CASE("toupper") {
        std::string test = "Convert To UPPERcase";
        xia::util::string::toupper(test);
        CHECK(test == "CONVERT TO UPPERCASE");
    }

    TEST_CASE("check number") {
      CHECK(xia::util::string::check_number("0") == true);
      CHECK(xia::util::string::check_number("-0") == true);
      CHECK(xia::util::string::check_number("1234567890") == true);
      CHECK(xia::util::string::check_number("-1234567890") == true);
      CHECK(xia::util::string::check_number("1.234567890") == true);
      CHECK(xia::util::string::check_number("-.1234") == true);
      CHECK(xia::util::string::check_number(".1e-9") == true);
      CHECK(xia::util::string::check_number("1e9") == true);
      CHECK(xia::util::string::check_number("1e-9") == true);
      CHECK(xia::util::string::check_number("-2e-9") == true);
      CHECK(xia::util::string::check_number("1.23456789a") == false);
      CHECK(xia::util::string::check_number("abc") == false);
      CHECK(xia::util::string::check_number("12abc456") == false);
      CHECK(xia::util::string::check_number_range("1") == true);
      CHECK(xia::util::string::check_number_range("all") == true);
      CHECK(xia::util::string::check_number_range("1-10") == true);
      CHECK(xia::util::string::check_number_range("1-a") == false);
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

    TEST_CASE("Path") {
        create_test_file();
        SUBCASE("basename") {
            CHECK(xia::util::path::basename(__FILE__) == "test_pixie_util.cpp");
#ifdef XIA_PIXIE_WINDOWS
            CHECK(xia::util::path::basename("\\a\\b\\c\\d") == "d");
            CHECK(xia::util::path::basename("\\a\\b\\c/d") == "d");
            CHECK(xia::util::path::basename("/a/b/c\\d") == "d");
#endif
            CHECK(xia::util::path::basename("/a/b/c/d") == "d");
        }
        SUBCASE("dirname") {
            CHECK(xia::util::path::basename(xia::util::path::dirname(__FILE__)) == "sdk");
#ifdef XIA_PIXIE_WINDOWS
            CHECK(xia::util::path::basename(xia::util::path::dirname("\\a\\b\\c\\d")) == "c");
            CHECK(xia::util::path::basename(xia::util::path::dirname("\\a\\b\\c/d")) == "c");
            CHECK(xia::util::path::basename(xia::util::path::dirname("/a/b/c\\d")) == "c");
#endif
            CHECK(xia::util::path::basename(xia::util::path::dirname("/a/b/c/d")) == "c");
        }
        SUBCASE("join") {
#ifdef XIA_PIXIE_WINDOWS
            #define ROOT "c:"
            const char* j_1 = "\\1\\2\\3\\4";
            const char* j_2 = "c:c\\d";
            const char* j_3 = "XYZ\\1\\2\\3\\4";
            const char* j_4 = "EFG\\1\\2\\3\\";
#else /* XIA_PIXIE_WINDOWS */
            #define ROOT "/"
            const char* j_1 = "/1/2/3/4";
            const char* j_2 = "/c/d";
            const char* j_3 = "XYZ/1/2/3/4";
            const char* j_4 = "EFG/1/2/3/";
#endif /* XIA_PIXIE_WINDOWS */
            std::string j;
            j = xia::util::path::join("", {"1", "2", "3", "4"});
            CHECK(j == j_1);
            j = xia::util::path::join("", {"a", "b", ROOT "c", "d"});
            CHECK(j == j_2);
            j = xia::util::path::join("XYZ", {"1", "2", "3", "4"});
            CHECK(j == j_3);
            j = xia::util::path::join("EFG", {"1", "2", "3", ""});
            CHECK(j == j_4);
        }
        SUBCASE("extension") {
            std::string test_good = "testfile.bin";
            std::string test_bad = "testfile";
            std::string test_end = "testfile.";
            std::string ext_good = xia::util::path::extension(test_good);
            std::string ext_bad = xia::util::path::extension(test_bad);
            std::string ext_end = xia::util::path::extension(test_end);
            CHECK(ext_good == "bin");
            CHECK(ext_bad == "testfile");
            CHECK(ext_end == "");
        }
        SUBCASE("exists") {
            CHECK(!xia::util::path::exists("/x/x/x/x/x/x"));
            CHECK(xia::util::path::exists(__FILE__));
            CHECK(xia::util::path::exists(xia::util::path::dirname(__FILE__)));
        }
        SUBCASE("isfile") {
            CHECK(!xia::util::path::isfile("/x/x/x/x/x/x"));
            CHECK(xia::util::path::isfile(__FILE__));
            CHECK(!xia::util::path::isfile(xia::util::path::dirname(__FILE__)));
        }
        SUBCASE("isdir") {
            CHECK(!xia::util::path::isdir("/x/x/x/x/x/x"));
            CHECK(!xia::util::path::isdir(__FILE__));
            CHECK(xia::util::path::isdir(xia::util::path::dirname(__FILE__)));
        }
    }

    TEST_CASE("Thread Workers") {
        using namespace std::chrono_literals;
        std::srand(static_cast<int>(std::time(nullptr)));
        const size_t num_threads = 10;
        xia::util::thread::workers threads;
        for (size_t t = 0; t < num_threads; ++t) {
            threads.emplace_back(
                [t, period = std::rand() % 3000]() {
                    std::this_thread::sleep_for(1ms * period);
                    if (t == 3) {
                        throw xia::pixie::error::error(
                            xia::pixie::error::code::not_supported, "t 3");
                    }
                    if (t == 5) {
                        throw xia::pixie::error::error(
                            xia::pixie::error::code::invalid_value, "t 5");
                    }
                });
        }
        for (auto& t : threads) {
            t.start();
        }
        bool waiter_called = false;
        std::vector<bool> finished_threads(num_threads, false);
        std::vector<xia::util::thread::error::code>
            error_threads(num_threads, xia::util::thread::error::code::success);
        xia::util::thread::waiter_func waiter =
            [&waiter_called](xia::util::thread::workers::size_type running) {
                if (running == 0) {
                    CHECK(running != 0);
                }
                waiter_called = true;
                std::this_thread::sleep_for(20ms);
            };
        xia::util::thread::finished_func thread_finished =
            [&finished_threads](xia::util::thread::workers::size_type t) {
                finished_threads[t] = true;
            };
        xia::util::thread::error_func thread_error =
            [&error_threads](
                xia::util::thread::workers::size_type t,
                xia::util::thread::error::code e) {
                error_threads[t] = e;
            };
        CHECK_THROWS_WITH_AS(
            xia::util::thread::wait_until_finished(
                threads, waiter, thread_finished, thread_error, "has errors"),
            "has errors", xia::pixie::error::error);
        CHECK(waiter_called == true);
        for (auto f : finished_threads) {
            CHECK(f == true);
        }
        CHECK(error_threads[3] == xia::pixie::error::code::not_supported);
        CHECK(error_threads[5] == xia::pixie::error::code::invalid_value);
        threads.clear();
        threads.resize(num_threads);
        for (size_t t = 0; t < threads.size(); ++t) {
            threads[t].body =
                [t, period = std::rand() % 3000]() {
                    std::this_thread::sleep_for(1ms * period);
                    if (t == 3) {
                        throw xia::pixie::error::error(
                            xia::pixie::error::code::not_supported, "t 3");
                    }
                    if (t == 5) {
                        throw xia::pixie::error::error(
                            xia::pixie::error::code::invalid_value, "t 5");
                    }
                };
            threads[t].start();
        }
        CHECK(xia::util::thread::wait_until_finished(threads, 20) !=
              xia::pixie::error::code::success);
    }

    TEST_CASE("file util") {
        xia::util::io::file file;
        CHECK(file.handle == -1);
        CHECK(file.valid() == false);

        const std::vector<uint8_t> data = {2, 3, 5};
        const std::vector<char> data2 = {'A', 'B', 'C'};
        std::vector<uint8_t> read;

        CHECK_THROWS_WITH_AS(
            file.write(data, data.size()), "io: file: write: not open",
            xia::pixie::error::error);
        CHECK_THROWS_WITH_AS(
            file.read(read, data.size()), "io: file: read: not open",
            xia::pixie::error::error);

        std::string time = xia::util::time::datetime_iso8601();
        xia::util::time::datetime_iso8601_as_filename(time);
        std::string filename = "test_file_" + time + ".txt";
        CHECK_THROWS_WITH_AS(
            file.open(filename, file.flag::rw),
            "io: file: open: No such file or directory",
            xia::pixie::error::error);
        CHECK_NOTHROW(file.create(filename, file.flag::rw));
        CHECK(file.valid() == true);
        CHECK_NOTHROW(file.close());
        CHECK(file.valid() == false);

        CHECK_NOTHROW(file.open(filename, file.flag::rw));
        CHECK(file.valid() == true);


        CHECK(file.write(data, data.size()) == data.size());
        CHECK(file.seek(0, file.seek_mode::cur) == data.size());
        CHECK(file.write(data) == data.size());
        CHECK(file.seek(0, file.seek_mode::cur) == data.size() * 2);

        CHECK(file.write(data2, data2.size()) == data2.size());

        size_t full_size = data.size() * 2 + data2.size();

        read.clear();
        read.resize(full_size);

        CHECK(file.seek(0, file.seek_mode::set) == 0);
        CHECK(file.read(read) == full_size);
        CHECK(read[0] == 2);
        CHECK(read[1] == 3);
        CHECK(read[2] == 5);
        CHECK(read[3] == 2);
        CHECK(read[4] == 3);
        CHECK(read[5] == 5);
        CHECK(read[6] == 'A');
        CHECK(read[7] == 'B');
        CHECK(read[8] == 'C');

        read.clear();
        read.resize(full_size);
        CHECK(file.seek(0, file.seek_mode::set) == 0);
        CHECK(file.read(read, full_size) == full_size);
        CHECK(read[0] == 2);
        CHECK(read[1] == 3);
        CHECK(read[2] == 5);
        CHECK(read[3] == 2);
        CHECK(read[4] == 3);
        CHECK(read[5] == 5);
        CHECK(read[6] == 'A');
        CHECK(read[7] == 'B');
        CHECK(read[8] == 'C');

        read.clear();
        read.resize(full_size);
        CHECK(file.seek(data.size(), file.seek_mode::set) == data.size());
        CHECK(file.read(read, full_size) == data.size() + data2.size());
        CHECK(read[0] == 2);
        CHECK(read[1] == 3);
        CHECK(read[2] == 5);
        CHECK(read[3] == 'A');
        CHECK(read[4] == 'B');
        CHECK(read[5] == 'C');

        CHECK_NOTHROW(file.close());
        CHECK(file.valid() == false);

        CHECK_NOTHROW(file.open(filename, file.flag::rw_trunc));
        CHECK(file.valid() == true);
        CHECK(file.seek(0, file.seek_mode::end) == 0);

        xia::util::thread::workers threads;
        for (size_t t = 0; t < 6; ++t) {
            threads.emplace_back(
                [&file]() {
                    file.close();
        });}
        for (auto& t : threads) {
            t.start();
        }
        CHECK(xia::util::thread::wait_until_finished(threads, 5) == xia::pixie::error::code::success);

        CHECK(file.valid() == false);
        CHECK(remove(filename.c_str()) == 0);
    }

    TEST_CASE("buffer stream") {
        xia::util::io::bufferstream bufstream;
        auto s1 = std::string("0123456789");
        {
            xia::util::io::bufferstream::reader r1(bufstream);
            CHECK_THROWS_WITH_AS(
                bufstream.push(s1),
                "bufferstream: not created",
                xia::pixie::error::error);
            CHECK_THROWS_WITH_AS(
                r1.tell(),
                "bufferstream: not created",
                xia::pixie::error::error);
            CHECK_THROWS_WITH_AS(
                r1.seek(100),
                "bufferstream: not created",
                xia::pixie::error::error);
            CHECK_NOTHROW(bufstream.create());
            CHECK(bufstream.available() == xia::util::io::bufferstream::default_page_size);
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK(r1.read(s1, 5) == 5);
            CHECK(s1 == "01234");
            CHECK(r1.tell() == 5);
            CHECK(r1.read(s1, 10) == 10);
            CHECK(s1 == "5678901234");
            CHECK(r1.tell() == 15);
            CHECK(r1.seek(1) == 1);
            CHECK(r1.read(s1, 10) == 10);
            CHECK(s1 == "1234567890");
            CHECK(r1.seek(100) == 50);
            CHECK(r1.size() == 50);
            CHECK(r1.full() == false);
            CHECK(r1.available() == xia::util::io::bufferstream::default_page_size - 50);
            s1.resize(r1.available());
            std::fill(s1.begin(), s1.end(), '#');
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK(r1.available() == 0);
            CHECK(
                r1.seek(xia::util::io::bufferstream::default_page_size - 50) ==
                xia::util::io::bufferstream::default_page_size - 50);
            CHECK(r1.read(s1, 100) == 50);
            s1 = '1';
            CHECK_NOTHROW(bufstream.push(s1));
            CHECK(r1.available() == xia::util::io::bufferstream::default_page_size - 1);
            CHECK(r1.size() == xia::util::io::bufferstream::default_page_size + 1);
        }
        CHECK_NOTHROW(bufstream.destroy());
        bufstream.create(200);
        {
            xia::util::io::bufferstream::reader r2(bufstream);
            CHECK(r2.available() == 200);
            CHECK(r2.full() == false);
            CHECK(r2.seek(0) == 0);
            s1 = "0123456789";
            for (size_t i = 0; i < 10; ++i) {
                CHECK_NOTHROW(bufstream.push(s1));
            }
            CHECK(r2.size() == 100);
            CHECK(r2.available() == 100);
            CHECK(r2.full() == false);
            for (size_t i = 0; i < 10; ++i) {
                CHECK_NOTHROW(bufstream.push(s1));
            }
            CHECK(r2.available() == 0);
            CHECK(r2.full() == true);
        }
    }

    TEST_CASE("version") {
        xia::util::version::version v1;
        CHECK(xia::util::version::to_string(v1) == "0.1.0");
        xia::util::version::version v2;
        {
            using namespace xia::util::version;
            v2 = "1.2.3"_version;
        }
        CHECK(xia::util::version::to_string(v2) == "1.2.3");
        CHECK(v1 != v2);
        CHECK(v1 < v2);
        v1.from_string("5.6.7");
        CHECK(v1 > v2);
    }

    TEST_CASE("token editor") {
        xia::util::string::token_editor te1("a,b,e,c");
        CHECK(te1.get() == "a,b,c,e");
        CHECK_NOTHROW(te1.add("d"));
        CHECK(te1.get() == "a,b,c,d,e");
        CHECK_NOTHROW(te1.add("z"));
        CHECK(te1.get() == "a,b,c,d,e,z");
        CHECK_NOTHROW(te1.remove("z"));
        CHECK(te1.get() == "a,b,c,d,e");
        xia::util::string::token_editor te2;
        CHECK_NOTHROW(te2 = "1,4,3,7,2,8");
        CHECK(te2.get() == "1,2,3,4,7,8");
        CHECK(te2.has("7") == true);
        CHECK(te2.has("5") == false);
        xia::util::string::token_editor te3("red:abc,blue:123,green:###", ',', false);
        CHECK(te3.get() == "red:abc,blue:123,green:###");
        CHECK_NOTHROW(te3.update("^red", "red:efg"));
        CHECK(te3.get() == "red:efg,blue:123,green:###");
        CHECK_NOTHROW(te3.update("^pink", "pink:xyz"));
        CHECK(te3.get() == "red:efg,blue:123,green:###,pink:xyz");
        auto gs1 = te3.get("^pink:");
        CHECK(gs1 == "pink:xyz");
        auto gs2 = te3.get("grey");
        CHECK(gs2.empty());
        auto gs3 = te3.get("123");
        CHECK(gs3 == "blue:123");
        std::string s1 = "aaa,ggg,jabc,jcde,jbcd";
        xia::util::string::token_editor te4(s1);
        CHECK(te4.get() == "aaa,ggg,jabc,jbcd,jcde");
        std::string s2 = "3335,3334,567,0123,jbcd";
        CHECK_NOTHROW(te4.set(s2));
        CHECK(te4.get() == "0123,3334,3335,567,jbcd");
        CHECK_NOTHROW(te4.tokens.clear());
        CHECK_NOTHROW(te4 = s2);
        CHECK(te4.get() == "0123,3334,3335,567,jbcd");
        CHECK_NOTHROW(te4.set("test:123,test:456,test:789"));
        CHECK_THROWS_WITH_AS(
            te4.update("^test:", "xxx"),
            "token edit: multiple tokens match: ^test:",
            std::runtime_error);
        CHECK_NOTHROW(
            te4.update("^test:", [](std::string& token) {
                if (token == "test:123") {
                    token = "test:abc";
                    return true;
                }
                return false;
            }));
        CHECK(te4.get() == "test:456,test:789,test:abc");
        CHECK_THROWS_WITH_AS(
            auto gse = te4.get("^test:"),
            "token edit: multiple tokens match: ^test:",
            std::runtime_error);
        CHECK_NOTHROW(te4.remove("^test:"));
        CHECK(te4.tokens.empty() == true);
    }

    TEST_CASE("linear fit") {
        SUBCASE("k = 0 line") {
            xia::util::numerics::linear_fit<double> fit;
            fit.update(1.0, 1.0);
            fit.update(2.0, 1.0);
            fit.update(3.0, 1.0);
            fit.calc();
            CHECK(fit.y(1.0) == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.y(3.0) == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.y(5.0) == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.k == doctest::Approx(0.0).epsilon(0.001));
            CHECK(fit.c == doctest::Approx(1.0).epsilon(0.001));
        }
        SUBCASE("k = 1 line") {
            xia::util::numerics::linear_fit<double> fit;
            fit.update(1.0, 1.0);
            fit.update(2.0, 2.0);
            fit.update(3.0, 3.0);
            fit.calc();
            CHECK(fit.y(1.0) == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.y(3.0) == doctest::Approx(3.0).epsilon(0.001));
            CHECK(fit.y(5.0) == doctest::Approx(5.0).epsilon(0.001));
            CHECK(fit.k == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.c == doctest::Approx(0.0).epsilon(0.001));
        }
        SUBCASE("k = 0.5 line") {
            xia::util::numerics::linear_fit<double> fit;
            fit.update(1.0, 1.0);
            fit.update(2.0, 1.0);
            fit.update(3.0, 2.0);
            fit.calc();
            CHECK(fit.y(3.0) == doctest::Approx(11.0/6.0).epsilon(0.001));
            CHECK(fit.y(5.0) == doctest::Approx(17.0/6.0).epsilon(0.001));
            CHECK(fit.y(7.0) == doctest::Approx(23.0/6.0).epsilon(0.001));
            CHECK(fit.k == doctest::Approx(0.5).epsilon(0.001));
            CHECK(fit.c == doctest::Approx(1.0/3.0).epsilon(0.001));
        }
        SUBCASE("k = -1 line") {
            xia::util::numerics::linear_fit<double> fit;
            fit.update(1.0, 3.0);
            fit.update(2.0, 2.0);
            fit.update(3.0, 1.0);
            fit.calc();
            CHECK(fit.y(1.0) == doctest::Approx(3.0).epsilon(0.001));
            CHECK(fit.y(3.0) == doctest::Approx(1.0).epsilon(0.001));
            CHECK(fit.y(5.0) == doctest::Approx(-1.0).epsilon(0.001));
            CHECK(fit.k == doctest::Approx(-1.0).epsilon(0.001));
            CHECK(fit.c == doctest::Approx(4.0).epsilon(0.001));
        }
    }
}
