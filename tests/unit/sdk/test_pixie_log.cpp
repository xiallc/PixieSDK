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

#include "doctest/doctest.h"
#include "pixie_error.hpp"
#include "pixie_log.hpp"

static const std::string test_message = "Curiouser and curiouser!";

TEST_SUITE("xia::log") {
    TEST_CASE("off") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("off", "", xia::log::level::off, false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::max_level) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("off");
    }
    TEST_CASE("error") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("error", "", xia::log::level::error, false);
        xia::logging::set_datetime_stamp("error", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("error");
    }
    TEST_CASE("warning") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("warning", "", xia::log::level::warning, false);
        xia::logging::set_datetime_stamp("warning", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("warning");
    }
    TEST_CASE("info") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("info", "", xia::log::level::info, false);
        xia::logging::set_datetime_stamp("info", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
            CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("info");
    }
    TEST_CASE("debug") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("debug", "", xia::log::level::debug, false);
        xia::logging::set_datetime_stamp("debug", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
            CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
            CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
            CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
            CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
            CHECK(test_stream.str() == "[DEBUG] " + test_message + "\n");
        test_stream.str("");

        xia::logging::stop("debug");
    }
}

TEST_SUITE("xia::logging") {
    TEST_CASE("level_logging") {
        xia::logging::start("level_logging", "", xia::log::level::off, false);
        SUBCASE("off") {
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("error") {
            xia::logging::set_level("level_logging", xia::log::level::error);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("warning") {
            xia::logging::set_level("level_logging", xia::log::level::warning);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("info") {
            xia::logging::set_level("level_logging", xia::log::level::info);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("debug") {
            xia::logging::set_level("level_logging", xia::log::level::debug);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("max_level") {
            xia::logging::set_level("level_logging", xia::log::level::max_level);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK(xia::logging::level_logging(xia::log::level::debug));
            CHECK(xia::logging::level_logging(xia::log::level::max_level));
        }
        xia::logging::stop("level_logging");
    }
    TEST_CASE("set_datetime_stamp") {
        xia::logging::start("set_datetime_stamp", "", xia::log::level::off, false);

        CHECK_THROWS_WITH_AS(xia::logging::set_datetime_stamp("unit_test", false),
                             "invalid log output name in set datetime stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_datetime_stamp");
    }
    TEST_CASE("set_line_numbers") {
        xia::logging::start("set_line_numbers", "", xia::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::logging::set_line_numbers("unit_test", false),
                             "invalid log output name in set line numbers",
                             xia::pixie::error::error);
        xia::logging::stop("set_line_numbers");
    }
    TEST_CASE("set_level") {
        xia::logging::start("set_level", "", xia::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::logging::set_level("unit_test", xia::log::info),
                             "invalid log output name in set level", xia::pixie::error::error);
        xia::logging::stop("set_level");
    }
    TEST_CASE("set_level_stamp") {
        xia::logging::start("set_level_stamp", "", xia::log::level::off, false);
        CHECK_THROWS_WITH_AS(xia::logging::set_level_stamp("unit_test", false),
                             "invalid log output name in set level stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_level_stamp");
    }
}