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

/** @file test_pixie_log.cpp
 * @brief Provides test coverage for our logging implementation
 */


#include <iostream>

#include <doctest/doctest.h>
#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/mib.hpp>

using log_entries = std::vector<std::string>;

static const std::string test_message = "Curiouser and curiouser!";

static constexpr size_t log_mib_entries_size = 1000;
static constexpr size_t log_mib_size = 50;

static void test_setup() {
    xia::logging::start("log", "test-log.txt", false);
    xia::logging::set_level(xia::log::level::info);
}

static void test_log_mib_pager(
    xia::mib::node& cmd, xia::mib::mib_walk_func& walker,
    bool forwards, log_entries& pages, log_entries& entries,
    size_t page_count) {
    const char* page_op = forwards ? "page-forward" : "page-back";
    for (size_t page = 0; page < page_count; ++page) {
        if (page != 0) {
            CHECK_NOTHROW(cmd.set(page_op));
        }
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == log_mib_size);
        pages.insert(
            forwards ? std::end(pages) : std::begin(pages),
            std::begin(entries), std::end(entries));
    }
    CHECK(pages.size() == page_count * log_mib_size);
}

TEST_SUITE("xia::log") {
    TEST_CASE("off") {
        test_setup();
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("off", "", false);
        xia::logging::set_level(xia::log::level::off);
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
        test_setup();
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("error", "", false);
        xia::logging::set_level(xia::log::level::error);
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
        test_setup();
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("warning", "", false);
        xia::logging::set_level(xia::log::level::warning);
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
        test_setup();
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("info", "", false);
        xia::logging::set_level(xia::log::level::info);
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
        test_setup();
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("debug", "", false);
        xia::logging::set_level(xia::log::level::debug);
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
        test_setup();
        std::string name = "level_logging";
        xia::logging::start(name, "", false);
        xia::logging::set_level(xia::log::level::off);
        SUBCASE("off") {
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("error") {
            xia::logging::set_level(xia::log::level::error);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("warning") {
            xia::logging::set_level(xia::log::level::warning);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("info") {
            xia::logging::set_level(xia::log::level::info);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("debug") {
            xia::logging::set_level(xia::log::level::debug);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("max_level") {
            xia::logging::set_level(xia::log::level::max_level);
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
        test_setup();
        xia::logging::start("set_datetime_stamp", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_datetime_stamp("unit_test", false),
                             "invalid log output name in set datetime stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_datetime_stamp");
    }
    TEST_CASE("set_line_numbers") {
        test_setup();
        xia::logging::start("set_line_numbers", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_line_numbers("unit_test", false),
                             "invalid log output name in set line numbers",
                             xia::pixie::error::error);
        xia::logging::stop("set_line_numbers");
    }
    TEST_CASE("set_level_stamp") {
        test_setup();
        xia::logging::start("set_level_stamp", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_level_stamp("unit_test", false),
                             "invalid log output name in set level stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_level_stamp");
    }
    TEST_CASE("Log MIB") {
        test_setup();
        xia::mib::node cmd;
        CHECK_NOTHROW(cmd = xia::mib::find("log.command"));
        CHECK_NOTHROW(xia::logging::set_level(xia::log::level::debug));
        CHECK_NOTHROW(cmd.set("on"));
        log_entries entries;
        xia::mib::mib_walk_func walker =
            [&entries](xia::mib::node& nod) {
                if (nod.str().length() > 42 && nod.name() != "log.command") {
                    entries.push_back(nod.str().substr(42));
                }
            };
        int count;
        /*
         * Follow
         */
        CHECK_NOTHROW(cmd.set("follow"));
        /*
         * Empty
         */
        CHECK_NOTHROW(cmd.set("flush"));
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 0);
        /*
         * Single message
         */
        CHECK_NOTHROW(cmd.set("flush"));
        xia::log(xia::log::level::info) << "message 1";
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 1);
        CHECK(entries[0] == "message 1");
        /*
         * Less than MIB of message
         */
        CHECK_NOTHROW(cmd.set("flush"));
        for (int i = 0; i < 20; ++i) {
            xia::log(xia::log::level::info) << "message " + std::to_string(i);
        }
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 20);
        for (size_t i = 0; i < entries.size(); ++i) {
            CHECK(entries[i] == "message "  + std::to_string(i));
        }
        /*
         * Overflow the entry table, max x2 + some
         */
        CHECK_NOTHROW(cmd.set("flush"));
        count = 2432;
        for (int i = 0; i < count; ++i) {
            xia::log(xia::log::level::info) << "lots of messages " + std::to_string(i);
        }
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 50);
        for (size_t i = 0; i < entries.size(); ++i) {
            CHECK(entries[i] == "lots of messages "  + std::to_string(count - 50 + i));
        }
        /*
         * Hold tests
         */
        CHECK_NOTHROW(cmd.set("hold"));
        CHECK_NOTHROW(cmd.set("flush"));
        xia::log(xia::log::level::info) << "message 1";
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 0);
        /*
         * Less than MIB of message
         */
        CHECK_NOTHROW(cmd.set("follow"));
        CHECK_NOTHROW(cmd.set("flush"));
        for (int i = 0; i < 20; ++i) {
            xia::log(xia::log::level::info) << "message " + std::to_string(i);
        }
        CHECK_NOTHROW(cmd.set("hold"));
        for (int i = 0; i < 20; ++i) {
            xia::log(xia::log::level::info) << "second message " + std::to_string(i);
        }
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 20);
        for (size_t i = 0; i < entries.size(); ++i) {
            CHECK(entries[i] == "message "  + std::to_string(i));
        }
        /*
         * Overflow the entry table
         */
        CHECK_NOTHROW(cmd.set("flush"));
        CHECK_NOTHROW(cmd.set("hold"));
        count = 2997;
        for (int i = 0; i < count; ++i) {
            xia::log(xia::log::level::info) << "lots of messages " + std::to_string(i);
        }
        entries.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        CHECK(entries.size() == 1);
        for (size_t i = 0; i < entries.size(); ++i) {
            CHECK(entries[i] == "lots of messages " + std::to_string(count - 1000));
        }
        /*
         * Paging, note, paging switches to holding
         */
        CHECK_NOTHROW(cmd.set("flush"));
        CHECK_NOTHROW(cmd.set("follow"));
        log_entries pages;
        count = 200;
        for (int i = 0; i < count; ++i) {
            xia::log(xia::log::level::info) << "messages " + std::to_string(i);
        }
        test_log_mib_pager(cmd, walker, false, pages, entries, count / log_mib_size);
        for (size_t i = 0; i < pages.size(); ++i) {
            CHECK(pages[i] == "messages "  + std::to_string(i));
        }
        pages.clear();
        CHECK_NOTHROW(cmd.set("page-forward"));
        test_log_mib_pager(cmd, walker, true, pages, entries, 2);
        for (size_t i = 0; i < pages.size(); ++i) {
            CHECK(pages[i] == "messages "  + std::to_string(i + log_mib_size));
        }
        /*
         * Overflow and page back without hitting the ends of the
         * entries table
         */
        CHECK_NOTHROW(cmd.set("flush"));
        count = log_mib_entries_size + 789;
        for (int i = 0; i < count; ++i) {
            xia::log(xia::log::level::info) << "message " + std::to_string(i);
        }
        CHECK_NOTHROW(cmd.set("end"));
        pages.clear();
        test_log_mib_pager(cmd, walker, false, pages, entries, 200 / log_mib_size);
        for (size_t i = 0; i < pages.size(); ++i) {
            CHECK(pages[i] == "message "  + std::to_string(count - 200 + i));
        }
        /*
         * Overflow and page back underolling the entries table
         */
        CHECK_NOTHROW(cmd.set("flush"));
        count = log_mib_entries_size + log_mib_size / 2;
        for (int i = 0; i < count; ++i) {
            xia::log(xia::log::level::info) << "message " + std::to_string(i);
        }
        CHECK_NOTHROW(cmd.set("end"));
        pages.clear();
        test_log_mib_pager(cmd, walker, false, pages, entries, 200 / log_mib_size);
        for (size_t i = 0; i < pages.size(); ++i) {
            CHECK(pages[i] == "message "  + std::to_string(count - 200 + i));
        }
        pages.clear();
        test_log_mib_pager(cmd, walker, true, pages, entries, 200 / log_mib_size);
        for (size_t i = 0; i < pages.size(); ++i) {
            CHECK(pages[i] == "message "  + std::to_string(count - 200 + i));
        }
        CHECK_NOTHROW(cmd.set("off"));
        /*
         * log.command status parser
         */
        xia::logging::log_mib_status status;
        CHECK_NOTHROW(
            status.parse(
                "count:1 mib-count:2 entries:1000 in:3 at:4 on:true mode:follow"));
        CHECK(status.count == 1);
        CHECK(status.mib_count == 2);
        CHECK(status.entries == 1000);
        CHECK(status.in == 3);
        CHECK(status.at == 4);
        CHECK(status.on == true);
        CHECK(status.mode == xia::logging::log_mib_status::follow);
        CHECK_NOTHROW(
            status.parse(
                "count:10 mib-count:20 entries:10000 in:30 at:40 on:false mode:hold"));
        CHECK(status.count == 10);
        CHECK(status.mib_count == 20);
        CHECK(status.entries == 10000);
        CHECK(status.in == 30);
        CHECK(status.at == 40);
        CHECK(status.on == false);
        CHECK(status.mode == xia::logging::log_mib_status::hold);
    }
}
