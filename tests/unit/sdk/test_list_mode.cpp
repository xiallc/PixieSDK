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

/** @file test_list_mode.cpp
 * @brief Tests related to the list_mode namespace
 */

#include <doctest/doctest.h>

#include <pixie/data/list_mode.hpp>
#include <pixie/error.hpp>

using namespace xia::pixie::data::list_mode;

TEST_SUITE("xia::pixie::list_mode") {
    TEST_CASE("event class") {
        static const std::string json_str = "{"
                                            "\"cfd_forced_trigger\":false,"
                                            "\"cfd_fractional_time\":0.0,"
                                            "\"cfd_trigger_source\":0,"
                                            "\"channel_number\":1,"
                                            "\"crate_id\":1,"
                                            "\"energy\":1235.0,"
                                            "\"energy_sums\":[],"
                                            "\"event_length\":7,"
                                            "\"event_time_high\":1111,"
                                            "\"event_time_low\":1234,"
                                            "\"external_time_high\":1111,"
                                            "\"external_time_low\":1111,"
                                            "\"filter_baseline\":0.0,"
                                            "\"finish_code\":false,"
                                            "\"header_length\":6,"
                                            "\"qdc\":[],"
                                            "\"slot_id\":1,"
                                            "\"time\":0.0,"
                                            "\"trace\":[196080109],"
                                            "\"trace_out_of_range\":false}";

        event evt;
        evt.energy = 1235;
        evt.slot_id = 1;
        evt.channel_number = 1;
        evt.crate_id = 1;
        evt.event_length = 7;
        evt.event_time_low = 1234;
        evt.event_time_high = 1111;
        evt.external_time_low = 1234;
        evt.external_time_high = 1111;
        evt.header_length = 6;
        evt.trace = std::vector<size_t>(1, 0xBAFF1ED);

        SUBCASE("Comparison operators") {
            event eventB = evt;
            CHECK(evt == eventB);

            eventB.energy = 1234;
            CHECK(evt != eventB);

            eventB.event_time_low = 1235;
            CHECK(evt < eventB);
            CHECK(eventB > evt);
        }

        SUBCASE("Verify 48-bit time calculations") {
            CHECK(evt.raw_time() == 4771708667090);
            CHECK(evt.external_time() == 4771708667090);
        }

        SUBCASE("Verify JSON output and input") {
            CHECK_THROWS_AS(event(""), xia::pixie::error::error);
            CHECK_THROWS_AS(event("{}"), xia::pixie::error::error);

            CHECK(event_as_json(evt) == json_str);
            CHECK(event(json_str) == evt);
        }

        SUBCASE("Verify streamed output") {
            std::ostringstream buf;
            buf << evt;
            CHECK(buf.str() == "crate: 1 slot: 1 channel: 1 time: 4771708667090 energy: 1235");
        }
    }
    TEST_CASE("Data Validity") {
        SUBCASE("Checks for data validity") {
            CHECK_THROWS_WITH_AS(decode_data_block(nullptr, 0, 30474, 250),
                                 "buffer pointed to an invalid location", xia::pixie::error::error);
            std::vector<uint32_t> empty(2, 0);
            CHECK_THROWS_WITH_AS(decode_data_block(empty.data(), empty.size(), 30474, 250),
                                 "minimum data buffer size is 4", xia::pixie::error::error);
            CHECK_THROWS_WITH_AS(decode_data_block(empty.data(), 4, 1, 250),
                                 "minimum supported firmware rev is 17562",
                                 xia::pixie::error::error);
        }
    }


    /*
     * Testing data decoding and encoding requires tons of different data combinations.
     * We're going to define most of the building blocks below. These will then be used
     * to construct all the various combinations that we'll need to perform our tests.
     */
    event init_event(double cfd_scale, size_t event_len, size_t header_len, size_t trig_source,
                     bool cfd_force, bool ets) {
        event expected;
        expected.cfd_forced_trigger = cfd_force;
        expected.cfd_fractional_time = 3085.0 / cfd_scale;
        expected.cfd_trigger_source = trig_source;
        expected.channel_number = 10;
        expected.crate_id = 0;
        expected.energy = 480.0;
        expected.energy_sums = {123, 456, 789};
        expected.event_length = event_len;
        expected.event_time_high = 4077;
        expected.event_time_low = 3735933136;
        expected.filter_baseline = 159.357;
        expected.finish_code = true;
        expected.header_length = header_len;
        expected.qdc = {147, 258, 369, 963, 852, 741, 159, 357};
        expected.slot_id = 2;
        expected.trace = {29,  31,  28, 30, 27, 30, 30, 31, 28, 29, 29, 30, 36, 86, 270,
                          426, 268, 80, 34, 29, 27, 32, 28, 29, 31, 32, 27, 33, 28, 29};
        expected.trace_length = expected.trace.size();
        expected.trace_out_of_range = true;
        if (ets) {
            expected.external_time_low = 538060824;
            expected.external_time_high = 33864;
        }
        return expected;
    }

    std::vector<uint32_t> generate_data(uint32_t word0, uint32_t word1, uint32_t word2,
                                        uint32_t word3, bool ets) {
        static const std::vector<uint32_t> packed_trace = {
            2031645, 1966108, 1966107, 2031646, 1900572, 1966109, 5636132, 27918606,
            5243148, 1900578, 2097179, 1900572, 2097183, 2162715, 1900572};
        static const std::vector<uint32_t> energy_sums = {123, 456, 789, 1126128484};
        static const std::vector<uint32_t> qdc = {147, 258, 369, 963, 852, 741, 159, 357};

        std::vector<uint32_t> data = {word0, word1, word2, word3};
        data.insert(data.end(), energy_sums.begin(), energy_sums.end());
        data.insert(data.end(), qdc.begin(), qdc.end());
        if (ets) {
            std::vector<uint32_t> vals = {538060824, 33864};
            data.insert(data.end(), vals.begin(), vals.end());
        }
        data.insert(data.end(), packed_trace.begin(), packed_trace.end());
        return data;
    }

    void check_decoded_data(const event& result, const event& expected) {
        CHECK(result.cfd_forced_trigger == expected.cfd_forced_trigger);
        CHECK(result.cfd_fractional_time == expected.cfd_fractional_time);
        CHECK(result.cfd_trigger_source == expected.cfd_trigger_source);
        CHECK(result.channel_number == expected.channel_number);
        CHECK(result.crate_id == expected.crate_id);
        CHECK(result.energy == expected.energy);
        CHECK(result.energy_sums == expected.energy_sums);
        CHECK(result.event_length == expected.event_length);
        CHECK(result.event_time_high == expected.event_time_high);
        CHECK(result.event_time_low == expected.event_time_low);
        CHECK(result.external_time_high == expected.external_time_high);
        CHECK(result.external_time_low == expected.external_time_low);
        CHECK(doctest::Approx(result.filter_baseline).epsilon(0.00001) == expected.filter_baseline);
        CHECK(result.finish_code == expected.finish_code);
        CHECK(result.header_length == expected.header_length);
        CHECK(result.qdc == expected.qdc);
        CHECK(result.slot_id == expected.slot_id);
        CHECK(result.time == expected.time);
        CHECK(result.trace == expected.trace);
        CHECK(result.trace_length == expected.trace_length);
        CHECK(result.trace_out_of_range == expected.trace_out_of_range);
    }

    TEST_CASE("17562-100") {
        auto evt = init_event(65536, 31, 16, 0, false, false);
        auto data =
            generate_data(3225354282, uint32_t(evt.event_time_low), 202182637, 1966560, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 17562, 100)[0], evt);
    }
    TEST_CASE("29432-100") {
        auto evt = init_event(65536, 31, 16, 0, false, false);
        auto data =
            generate_data(2151612458, uint32_t(evt.event_time_low), 202182637, 1999328, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 29432, 100)[0], evt);
    }
    TEST_CASE("30474-100") {
        auto evt = init_event(32768, 31, 16, 0, true, false);
        auto data =
            generate_data(2151612458, uint32_t(evt.event_time_low), 2349666285, 1999328, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 30474, 100)[0], evt);
    }
    TEST_CASE("34688-100") {
        auto evt = init_event(32768, 33, 18, 0, true, true);
        auto data =
            generate_data(2151882794, uint32_t(evt.event_time_low), 2349666285, 2149450208, true);
        check_decoded_data(decode_data_block(data.data(), data.size(), 34688, 100)[0], evt);
    }
    TEST_CASE("20466-250") {
        auto evt = init_event(65536, 31, 16, 0, false, false);
        auto data =
            generate_data(3225354282, uint32_t(evt.event_time_low), 202182637, 1966560, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 20466, 250)[0], evt);
    }
    TEST_CASE("27361-250") {
        auto evt = init_event(32768, 31, 16, 1, false, false);
        auto data =
            generate_data(3225354282, uint32_t(evt.event_time_low), 2349666285, 1966560, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 27361, 250)[0], evt);
    }
    TEST_CASE("29432-250") {
        auto evt = init_event(32768, 31, 16, 1, false, false);
        auto data =
            generate_data(2151612458, uint32_t(evt.event_time_low), 2349666285, 1999328, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 29432, 250)[0], evt);
    }
    TEST_CASE("30474-250") {
        auto evt = init_event(16384, 31, 16, 1, true, false);
        auto data =
            generate_data(2151612458, uint32_t(evt.event_time_low), 3423408109, 1999328, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 30474, 250)[0], evt);
    }
    TEST_CASE("34688-250") {
        auto evt = init_event(16384, 33, 18, 1, true, true);
        auto data =
            generate_data(2151882794, uint32_t(evt.event_time_low), 3423408109, 2149450208, true);
        check_decoded_data(decode_data_block(data.data(), data.size(), 34688, 250)[0], evt);
    }
    TEST_CASE("46540-250") {
        auto evt = init_event(16384, 33, 18, 1, true, true);
        auto data =
            generate_data(2151882890, uint32_t(evt.event_time_low), 3423408109, 2149450208, true);
        check_decoded_data(decode_data_block(data.data(), data.size(), 46540, 250)[0], evt);
    }
    TEST_CASE("29432-500") {
        auto evt = init_event(8192, 31, 16, 6, false, false);
        auto data =
            generate_data(2151612458, uint32_t(evt.event_time_low), 3423408109, 1999328, false);
        check_decoded_data(decode_data_block(data.data(), data.size(), 29432, 500)[0], evt);
    }
    TEST_CASE("34688-500") {
        auto evt = init_event(8192, 33, 18, 6, false, true);
        auto data =
            generate_data(2151882794, uint32_t(evt.event_time_low), 3423408109, 2149450208, true);
        check_decoded_data(decode_data_block(data.data(), data.size(), 34688, 500)[0], evt);
    }
}