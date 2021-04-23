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
/// @date April 20, 2021

#include <iostream>

#include "doctest.h"
#include "pixie_error.hpp"
#include "pixie_log.hpp"

static const std::string test_message = "Curiouser and curiouser!";

TEST_SUITE("xia::pixie::log") {
    TEST_CASE("off") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::pixie::logging::start("off", "", xia::pixie::log::level::off, false);
        std::cout.rdbuf(old);

        xia::pixie::log(xia::pixie::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::error) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::max_level) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::logging::stop("off");
    }
    TEST_CASE("error") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::pixie::logging::start("error", "", xia::pixie::log::level::error, false);
        xia::pixie::logging::set_datetime_stamp("error", false);
        std::cout.rdbuf(old);

        xia::pixie::log(xia::pixie::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::logging::stop("error");
    }
    TEST_CASE("warning") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::pixie::logging::start("warning", "", xia::pixie::log::level::warning, false);
        xia::pixie::logging::set_datetime_stamp("warning", false);
        std::cout.rdbuf(old);

        xia::pixie::log(xia::pixie::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::info) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::debug) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::logging::stop("warning");
    }
    TEST_CASE("info") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::pixie::logging::start("info", "", xia::pixie::log::level::info, false);
        xia::pixie::logging::set_datetime_stamp("info", false);
        std::cout.rdbuf(old);

        xia::pixie::log(xia::pixie::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::info) << test_message;
            CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::debug) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::logging::stop("info");
    }
    TEST_CASE("debug") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::pixie::logging::start("debug", "", xia::pixie::log::level::debug, false);
        xia::pixie::logging::set_datetime_stamp("debug", false);
        std::cout.rdbuf(old);

        xia::pixie::log(xia::pixie::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::info) << test_message;
            CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::log(xia::pixie::log::level::debug) << test_message;
            CHECK(test_stream.str() == "[DEBUG] " + test_message + "\n");
        test_stream.str("");

        xia::pixie::logging::stop("debug");
    }
}

TEST_SUITE("xia::pixie::logging") {
    TEST_CASE("level_logging") {
        SUBCASE("off") {
            xia::pixie::logging::start("off", "", xia::pixie::log::level::off, false);
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("off");
        }
        SUBCASE("error") {
            xia::pixie::logging::set_level("error", xia::pixie::log::level::error);
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("error");
        }
        SUBCASE("warning") {
            xia::pixie::logging::set_level("warning", xia::pixie::log::level::warning);
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("warning");
        }
        SUBCASE("info") {
            xia::pixie::logging::set_level("info", xia::pixie::log::level::info);
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("info");
        }
        SUBCASE("debug") {
            xia::pixie::logging::set_level("debug", xia::pixie::log::level::debug);
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("debug");
        }
        SUBCASE("max_level") {
            xia::pixie::logging::set_level("max_level", xia::pixie::log::level::max_level);
            CHECK_FALSE(xia::pixie::logging::level_logging(xia::pixie::log::level::off));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::error));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::warning));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::info));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::debug));
            CHECK(xia::pixie::logging::level_logging(xia::pixie::log::level::max_level));
            xia::pixie::logging::stop("max_level");
        }
    }
    TEST_CASE("set_datetime_stamp") {
        xia::pixie::logging::start("set_datetime_stamp", "", xia::pixie::log::level::off, false);

        CHECK_THROWS_WITH_AS(xia::pixie::logging::set_datetime_stamp("unit_test", false),
                             "invalid log output name in set datetime stamp",
                             xia::pixie::error::error);
        xia::pixie::logging::stop("set_datetime_stamp");
    }
    TEST_CASE("set_line_numbers") {
        xia::pixie::logging::start("set_line_numbers", "", xia::pixie::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::pixie::logging::set_line_numbers("unit_test", false),
                             "invalid log output name in set line numbers",
                             xia::pixie::error::error);
        xia::pixie::logging::stop("set_line_numbers");
    }
    TEST_CASE("set_level") {
        xia::pixie::logging::start("set_level", "", xia::pixie::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::pixie::logging::set_level("unit_test", xia::pixie::log::info),
                             "invalid log output name in set level", xia::pixie::error::error);
        xia::pixie::logging::stop("set_level");
    }
    TEST_CASE("set_level_stamp") {
        xia::pixie::logging::start("set_level_stamp", "", xia::pixie::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::pixie::logging::set_level_stamp("unit_test", false),
                             "invalid log output name in set level stamp",
                             xia::pixie::error::error);
        xia::pixie::logging::stop("set_level_stamp");
    }
}