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

TEST_SUITE("xia::pixie::list_mode") {
    TEST_CASE("event class") {
        static const std::string json_str = "{"
                                            "\"cfd_forced_trigger\":false,"
                                            "\"cfd_time\":0,"
                                            "\"cfd_trigger_source\":false,"
                                            "\"channel_number\":1,"
                                            "\"crate_id\":1,"
                                            "\"energy\":1235.0,"
                                            "\"energy_sums\":[],"
                                            "\"event_time_high\":1111,"
                                            "\"event_time_low\":1234,"
                                            "\"external_time_high\":1111,"
                                            "\"external_time_low\":1111,"
                                            "\"filter_baseline\":0.0,"
                                            "\"finish_code\":false,"
                                            "\"qdc\":[],"
                                            "\"slot_id\":1,"
                                            "\"time\":0.0,"
                                            "\"trace\":[196080109],"
                                            "\"trace_out_of_range\":false}";

        xia::pixie::data::list_mode::event event;
        event.energy = 1235;
        event.slot_id = 1;
        event.channel_number = 1;
        event.crate_id = 1;
        event.event_time_low = 1234;
        event.event_time_high = 1111;
        event.external_time_low = 1234;
        event.external_time_high = 1111;
        event.trace = std::vector<size_t>(1, 0xBAFF1ED);

        SUBCASE("Comparison operators") {
            xia::pixie::data::list_mode::event eventB = event;
            CHECK(event == eventB);

            eventB.energy = 1234;
            CHECK(event != eventB);

            eventB.event_time_low = 1235;
            CHECK(event < eventB);
            CHECK(eventB > event);
        }

        SUBCASE("Verify 48-bit time calculations") {
            CHECK(event.raw_time() == 4771708667090);
            CHECK(event.external_time() == 4771708667090);
        }

        SUBCASE("Verify JSON output and input") {
            CHECK_THROWS_AS(xia::pixie::data::list_mode::event(""),
                            xia::pixie::error::error);
            CHECK_THROWS_AS(xia::pixie::data::list_mode::event("{}"),
                            xia::pixie::error::error);

            CHECK(event.as_json() == json_str);
            CHECK(xia::pixie::data::list_mode::event(json_str) == event);
        }

        SUBCASE("Verify streamed output") {
            std::ostringstream buf;
            buf << event;
            CHECK(buf.str() == "crate: 1 slot: 1 channel: 1 time: 4771708667090 energy: 1235");
        }
    }
}
