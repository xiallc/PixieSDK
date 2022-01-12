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
            CHECK_THROWS_AS(xia::pixie::data::list_mode::event(""), xia::pixie::error::error);
            CHECK_THROWS_AS(xia::pixie::data::list_mode::event("{}"), xia::pixie::error::error);

            CHECK(event_as_json(evt) == json_str);
            CHECK(event(json_str) == evt);
        }

        SUBCASE("Verify streamed output") {
            std::ostringstream buf;
            buf << evt;
            CHECK(buf.str() == "crate: 1 slot: 1 channel: 1 time: 4771708667090 energy: 1235");
        }
    }
    TEST_CASE("Data decoding") {
        std::vector<uint32_t> data = {540717, 3735933136, 4077, 480};
        auto evts = decode_data_block(data.data(), data.size(), 30474, 250);
        CHECK(evts[0] == event("{"
                               "\"cfd_forced_trigger\":false,"
                               "\"cfd_fractional_time\":0.0,"
                               "\"cfd_trigger_source\":0,"
                               "\"channel_number\":13,"
                               "\"crate_id\":0,"
                               "\"energy\":480.0,"
                               "\"energy_sums\":[],"
                               "\"event_length\":7,"
                               "\"event_time_high\":4077,"
                               "\"event_time_low\":3735933136,"
                               "\"external_time_high\":0,"
                               "\"external_time_low\":0,"
                               "\"filter_baseline\":0.0,"
                               "\"finish_code\":false,"
                               "\"header_length\":6,"
                               "\"qdc\":[],"
                               "\"slot_id\":2,"
                               "\"time\":0.0,"
                               "\"trace\":[],"
                               "\"trace_out_of_range\":false}"));
    }
}