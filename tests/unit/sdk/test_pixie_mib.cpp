/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
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

/** @file test_pixie_mib.cpp
 * @brief Provides test coverage for the MIB.
 */

#include <doctest/doctest.h>
#include <pixie/mib.hpp>

TEST_SUITE("xia::pixie::mib") {
    TEST_CASE("Empty MIB") {
        CHECK(xia::mib::contains("a.b.c") == false);
        CHECK_THROWS_WITH_AS(xia::mib::node(std::string("a.b.c")),
                             "mib::find: mib not found: a.b.c", xia::mib::error);
        CHECK_THROWS_WITH_AS(xia::mib::node("a.b.c"),
                             "mib::find: mib not found: a.b.c", xia::mib::error);
    }
    TEST_CASE("Node checks") {
        CHECK_THROWS_WITH_AS(xia::mib::node(std::string(), xia::mib::type::integer),
                             "mib::add: name empty", xia::mib::error);
        CHECK_THROWS_WITH_AS(xia::mib::node(nullptr, xia::mib::type::integer),
                             "mib::add: name empty", xia::mib::error);
        CHECK_THROWS_WITH_AS(
            { auto e = xia::mib::node(std::string()); (void) e;},
            "mib::add: name empty", xia::mib::error);
        CHECK_THROWS_WITH_AS(xia::mib::node(nullptr),
                             "mib::add: name empty", xia::mib::error);
    }
    TEST_CASE("Type checks") {
        CHECK(xia::mib::get_type<xia::mib::string>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<xia::mib::stringp>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<const char*>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<char*>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<char[123]>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<const char[123]>() == xia::mib::type::string);
        CHECK(xia::mib::get_type<xia::mib::boolean>() == xia::mib::type::boolean);
        CHECK(xia::mib::get_type<bool>() == xia::mib::type::boolean);
        CHECK(xia::mib::get_type<xia::mib::integer>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<int>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<long>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<long long>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<xia::mib::uinteger>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<unsigned int>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<unsigned long>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<unsigned long long>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<xia::mib::real>() == xia::mib::type::real);
        CHECK(xia::mib::get_type<double>() == xia::mib::type::real);
        CHECK(xia::mib::get_type<float>() == xia::mib::type::real);
        CHECK(xia::mib::get_type<xia::mib::timestamp>() == xia::mib::type::timestamp);
        CHECK(xia::mib::get_type<std::atomic_int>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<std::atomic_long>() == xia::mib::type::integer);
        CHECK(xia::mib::get_type<std::atomic_uint>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<std::atomic_ulong>() == xia::mib::type::uinteger);
        CHECK(xia::mib::get_type<std::atomic_size_t>() == xia::mib::type::uinteger);
    }
    std::string ss("hello world");
    const char* sp = "HELLO WORLD";
    const char* sps = "\"HELLO WORLD\"";
    const unsigned int ui = 1;
    xia::mib::timestamp ts = 1 ;
    TEST_CASE("MIB: string") {
        CHECK_NOTHROW(xia::mib::node("s.1", xia::mib::type::string));
        CHECK_NOTHROW(xia::mib::node("s.1"));
        auto s1 = xia::mib::node("s.1");
        CHECK(s1.is_enabled() == true);
        CHECK(s1.type_is() == xia::mib::type::string);
        CHECK(s1.type_is() != xia::mib::type::boolean);
        CHECK_NOTHROW(s1 = ss);
        CHECK(s1 == ss);
        CHECK(s1 != sp);
        CHECK(s1 > sp);
        CHECK(s1 >= sp);
        CHECK(s1.str() == '"' + ss + '"');
        CHECK(s1.str(true) == '"' + ss + "\" (rw unlocked)");
        CHECK_NOTHROW(s1 = sp);
        CHECK(s1 != ss);
        CHECK(s1 == sp);
        CHECK(s1 < ss);
        CHECK(s1 <= ss);
        CHECK(s1.str() == std::string(sps));
        CHECK_NOTHROW(s1 = "check for char[19]");
        CHECK(s1.write_locked() == false);
        CHECK_NOTHROW(s1.lock_writes());
        CHECK(s1.write_locked() == true);
        CHECK_THROWS_WITH_AS(s1 = "write while locked",
                             "mib::node::set: MIB is write locked", xia::mib::error);
        CHECK_NOTHROW(s1.unlock_writes());
        CHECK(s1.write_locked() == false);
        CHECK_NOTHROW(s1 = "write while locked");
        CHECK_THROWS_WITH_AS(s1 = true, "mib::node::check: s.1: invalid type: 0/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = false, "mib::node::check: s.1: invalid type: 0/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = int(1), "mib::node::check: s.1: invalid type: 0/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1, "mib::node::check: s.1: invalid type: 0/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1L, "mib::node::check: s.1: invalid type: 0/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1LL, "mib::node::check: s.1: invalid type: 0/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = ui, "mib::node::check: s.1: invalid type: 0/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1U, "mib::node::check: s.1: invalid type: 0/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1UL, "mib::node::check: s.1: invalid type: 0/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1ULL, "mib::node::check: s.1: invalid type: 0/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = 1.0, "mib::node::check: s.1: invalid type: 0/4", xia::mib::error);
        CHECK_THROWS_WITH_AS(s1 = ts, "mib::node::check: s.1: invalid type: 0/5", xia::mib::error);
    }
    TEST_CASE("MIB: boolean") {
        CHECK_NOTHROW(xia::mib::node("b.1", xia::mib::type::boolean, false));
        CHECK_NOTHROW(xia::mib::node("b.1"));
        auto b1 = xia::mib::node("b.1");
        CHECK(b1.is_enabled() == false);
        CHECK_NOTHROW(b1.enable());
        CHECK(b1.type_is() == xia::mib::type::boolean);
        CHECK(b1.type_is() != xia::mib::type::string);
        CHECK(b1.is_enabled() == true);
        CHECK_THROWS_WITH_AS(b1 = ss, "mib::node::check: b.1: invalid type: 1/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = sp, "mib::node::check: b.1: invalid type: 1/0", xia::mib::error);
        CHECK_NOTHROW(b1 = true);
        CHECK(b1 == true);
        CHECK(b1 != false);
        CHECK(b1 > false);
        CHECK(b1 >= true);
        CHECK(b1.str() == "true");
        CHECK_NOTHROW(b1 = false);
        CHECK(b1 == false);
        CHECK(b1 != true);
        CHECK(b1 < true);
        CHECK(b1 <= false);
        CHECK(b1.str() == "false");
        CHECK_THROWS_WITH_AS(b1 = int(1), "mib::node::check: b.1: invalid type: 1/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1, "mib::node::check: b.1: invalid type: 1/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1L, "mib::node::check: b.1: invalid type: 1/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1LL, "mib::node::check: b.1: invalid type: 1/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = ui, "mib::node::check: b.1: invalid type: 1/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1U, "mib::node::check: b.1: invalid type: 1/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1UL, "mib::node::check: b.1: invalid type: 1/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1ULL, "mib::node::check: b.1: invalid type: 1/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = 1.0, "mib::node::check: b.1: invalid type: 1/4", xia::mib::error);
        CHECK_THROWS_WITH_AS(b1 = ts, "mib::node::check: b.1: invalid type: 1/5", xia::mib::error);
    }
    TEST_CASE("MIB: integer") {
        CHECK_NOTHROW(xia::mib::node("i.1", xia::mib::type::integer));
        CHECK_NOTHROW(xia::mib::node("i.1"));
        auto i1 = xia::mib::node("i.1");
        CHECK(i1.type_is() == xia::mib::type::integer);
        CHECK(i1.type_is() != xia::mib::type::string);
        CHECK(i1.is_enabled() == true);
        CHECK_THROWS_WITH_AS(i1 = ss, "mib::node::check: i.1: invalid type: 2/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(i1 = sp, "mib::node::check: i.1: invalid type: 2/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(i1 = true, "mib::node::check: i.1: invalid type: 2/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(i1 = false, "mib::node::check: i.1: invalid type: 2/1", xia::mib::error);
        CHECK_NOTHROW(i1 = int(1));
        CHECK(i1 == 1);
        CHECK(i1 == 1U);
        CHECK(i1 == 1LL);
        CHECK(i1 == 1ULL);
        CHECK(i1 < 2);
        CHECK(i1 <= 1);
        CHECK(i1 > -1);
        CHECK(i1 >= -1);
        CHECK(i1.str() == "1");
        CHECK_NOTHROW(i1 = -1);
        CHECK(i1.str() == "-1");
        CHECK_NOTHROW(i1 = 1);
        CHECK_NOTHROW(i1 = 1L);
        CHECK_NOTHROW(i1 = 1LL);
        CHECK_NOTHROW(i1 = ui);
        CHECK_NOTHROW(i1 = 1U);
        CHECK_NOTHROW(i1 = 1UL);
        CHECK_NOTHROW(i1 = 1ULL);
        CHECK_THROWS_WITH_AS(i1 = 1.0, "mib::node::check: i.1: invalid type: 2/4", xia::mib::error);
        CHECK_THROWS_WITH_AS(i1 = ts, "mib::node::check: i.1: invalid type: 2/5", xia::mib::error);
    }
    TEST_CASE("MIB: uinteger") {
        CHECK_NOTHROW(xia::mib::node("u.1", xia::mib::type::uinteger));
        CHECK_NOTHROW(xia::mib::node("u.1"));
        auto u1 = xia::mib::node("u.1");
        CHECK(u1.type_is() == xia::mib::type::uinteger);
        CHECK(u1.type_is() != xia::mib::type::string);
        CHECK(u1.is_enabled() == true);
        CHECK_THROWS_WITH_AS(u1 = ss, "mib::node::check: u.1: invalid type: 3/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(u1 = sp, "mib::node::check: u.1: invalid type: 3/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(u1 = true, "mib::node::check: u.1: invalid type: 3/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(u1 = false, "mib::node::check: u.1: invalid type: 3/1", xia::mib::error);
        CHECK_NOTHROW(u1 = int(1));
        CHECK_NOTHROW(u1 = 1);
        CHECK_NOTHROW(u1 = 1L);
        CHECK_NOTHROW(u1 = 1LL);
        CHECK_NOTHROW(u1 = ui);
        CHECK(u1 == ui);
        CHECK(u1 < ui + 1);
        CHECK(u1 <= ui);
        CHECK(u1 > 0);
        CHECK(u1 >= ui);
        CHECK(u1.str() == "1");
        CHECK_NOTHROW(u1 = 0x100);
        CHECK(u1.str() == "256");
        CHECK_NOTHROW(u1 = 1U);
        CHECK_NOTHROW(u1 = 1UL);
        CHECK_NOTHROW(u1 = 1ULL);
        CHECK_THROWS_WITH_AS(u1 = 1.0, "mib::node::check: u.1: invalid type: 3/4", xia::mib::error);
        CHECK_THROWS_WITH_AS(u1 = ts, "mib::node::check: u.1: invalid type: 3/5", xia::mib::error);
    }
    TEST_CASE("MIB: real") {
        CHECK_NOTHROW(xia::mib::node("r.1", xia::mib::type::real));
        CHECK_NOTHROW(xia::mib::node("r.1"));
        auto r1 = xia::mib::node("r.1");
        CHECK(r1.type_is() == xia::mib::type::real);
        CHECK(r1.type_is() != xia::mib::type::string);
        CHECK(r1.is_enabled() == true);
        CHECK_THROWS_WITH_AS(r1 = ss, "mib::node::check: r.1: invalid type: 4/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = sp, "mib::node::check: r.1: invalid type: 4/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = true, "mib::node::check: r.1: invalid type: 4/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = false, "mib::node::check: r.1: invalid type: 4/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = int(1), "mib::node::check: r.1: invalid type: 4/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1, "mib::node::check: r.1: invalid type: 4/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1L, "mib::node::check: r.1: invalid type: 4/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1LL, "mib::node::check: r.1: invalid type: 4/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = ui, "mib::node::check: r.1: invalid type: 4/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1U, "mib::node::check: r.1: invalid type: 4/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1UL, "mib::node::check: r.1: invalid type: 4/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(r1 = 1ULL, "mib::node::check: r.1: invalid type: 4/3", xia::mib::error);
        CHECK_NOTHROW(r1 = 1.0);
        CHECK(r1.str() == "1.000000000");
        CHECK_NOTHROW(r1 = 1.123456789);
        CHECK(r1.str() == "1.123456789");
        CHECK_THROWS_WITH_AS(r1 = ts, "mib::node::check: r.1: invalid type: 4/5", xia::mib::error);
    }
    TEST_CASE("MIB: timestamp") {
        CHECK_NOTHROW(xia::mib::node("t.1", xia::mib::type::timestamp));
        CHECK_NOTHROW(xia::mib::node("t.1"));
        auto t1 = xia::mib::node("t.1");
        CHECK(t1.type_is() == xia::mib::type::timestamp);
        CHECK(t1.type_is() != xia::mib::type::string);
        CHECK(t1.is_enabled() == true);
        CHECK_THROWS_WITH_AS(t1 = ss, "mib::node::check: t.1: invalid type: 5/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = sp, "mib::node::check: t.1: invalid type: 5/0", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = true, "mib::node::check: t.1: invalid type: 5/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = false, "mib::node::check: t.1: invalid type: 5/1", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = int(1), "mib::node::check: t.1: invalid type: 5/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1, "mib::node::check: t.1: invalid type: 5/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1L, "mib::node::check: t.1: invalid type: 5/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1LL, "mib::node::check: t.1: invalid type: 5/2", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = ui, "mib::node::check: t.1: invalid type: 5/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1U, "mib::node::check: t.1: invalid type: 5/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1UL, "mib::node::check: t.1: invalid type: 5/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1ULL, "mib::node::check: t.1: invalid type: 5/3", xia::mib::error);
        CHECK_THROWS_WITH_AS(t1 = 1.0, "mib::node::check: t.1: invalid type: 5/4", xia::mib::error);
        CHECK_NOTHROW(t1 = ts);
        CHECK(t1.str() == "0.000000001");
        CHECK_NOTHROW(t1 = xia::mib::timestamp(1123456789));
        CHECK(t1.str() == "1.123456789");
    }
    TEST_CASE("MIB: add, contains") {
        std::string a1("a.1");
        CHECK(xia::mib::contains(a1) == false);
        CHECK_NOTHROW(xia::mib::add(a1, xia::mib::type::string));
        CHECK_THROWS_WITH_AS(
            xia::mib::add(a1, xia::mib::type::integer),
            "mib::add: mib already registered: a.1", xia::mib::error);
        CHECK(xia::mib::contains(a1) == true);
        CHECK(xia::mib::contains("a.2") == false);
        CHECK_NOTHROW(xia::mib::add("a.2", xia::mib::type::string));
        CHECK_THROWS_WITH_AS(
            xia::mib::add("a.2", xia::mib::type::integer),
            "mib::add: mib already registered: a.2", xia::mib::error);
        CHECK(xia::mib::contains("a.2") == true);
    }
    TEST_CASE("MIB: read-only") {
        CHECK_NOTHROW(xia::mib::add_ro_str("s.2.ro", "s2 read-only"));
        CHECK_NOTHROW(xia::mib::add_ro_bool("b.2.ro", true));
        CHECK_NOTHROW(xia::mib::add_ro_int("i.2.ro", 100));
        CHECK_NOTHROW(xia::mib::add_ro_uint("u.2.ro", 1000U));
        CHECK_NOTHROW(xia::mib::add_ro_real("r.2.ro", 3.33333));
        CHECK_NOTHROW(xia::mib::add_ro_timestamp("t.2.ro", xia::mib::timestamp(987654321)));
        auto s = xia::mib::node("s.2.ro");
        CHECK_THROWS_WITH_AS(s = "abc", "mib::node::set: MIB is read-only", xia::mib::error);
        CHECK_THROWS_WITH_AS(s.lock_writes(), "mib::node::lock-write: MIB is read-only", xia::mib::error);
    }
    TEST_CASE("MIB: events") {
        auto s1 = xia::mib::node("s.1");
        CHECK(s1.has_event_func(xia::mib::event::set) == false);
        CHECK(s1.has_event_func(xia::mib::event::get) == false);
        CHECK(s1.has_event_func(xia::mib::event::timer) == false);
        std::string s1_set = "no set called";
        xia::mib::event_func set_func =
            [&s1, &s1_set](xia::mib::event event, xia::mib::type type, xia::mib::data_type& data) {
                CHECK(type == xia::mib::type::string);
                CHECK(event == xia::mib::event::set);
                std::string s = s1;
                CHECK(s == data.s);
                s1_set = s1.get<xia::mib::string>();
            };
        s1.set_event_func(xia::mib::event::set, set_func);
        std::string s1_get = "no get called";
        xia::mib::event_func get_func =
            [&s1, &s1_get](xia::mib::event event, xia::mib::type type, xia::mib::data_type& data) {
                CHECK(type == xia::mib::type::string);
                CHECK(event == xia::mib::event::get);
                s1_get = s1.get<xia::mib::string>();
                CHECK(s1_get == data.s);
            };
        s1.set_event_func(xia::mib::event::get, get_func);
        s1 = "setting s1";
        CHECK(s1_set != "no set called");
        CHECK(s1_set == "setting s1");
        CHECK(s1_get == "no get called");
        std::string s1_read = s1;
        CHECK(s1_get != "no get called");
        CHECK(s1_get == "setting s1");
        CHECK(s1.has_event_func(xia::mib::event::set) == true);
        CHECK(s1.has_event_func(xia::mib::event::get) == true);
        /* need to clear these events because local references are captured */
        CHECK_NOTHROW(s1.clear_event_func(xia::mib::event::set));
        CHECK_NOTHROW(s1.clear_event_func(xia::mib::event::get));
        CHECK(s1.has_event_func(xia::mib::event::set) == false);
        CHECK(s1.has_event_func(xia::mib::event::get) == false);
        CHECK(s1.has_event_func(xia::mib::event::timer) == false);
    }
    TEST_CASE("MIB: walk") {
        std::vector<std::string> mibs;
        xia::mib::mib_walk_func walker =
            [&mibs](xia::mib::node& nod) {
                mibs.push_back(nod.name());
            };
        CHECK_NOTHROW(xia::mib::walk(walker));
        const char* mib_list[] = {
            "a.1",
            "a.2",
            "b.1",
            "b.2.ro",
            "i.1",
            "i.2.ro",
            "r.1",
            "r.2.ro",
            "s.1",
            "s.2.ro",
            "t.1",
            "t.2.ro",
            "u.1",
            "u.2.ro",
        };
        const size_t npc = sizeof(mib_list) / sizeof(mib_list[0]);
        CHECK(npc == mibs.size());
        size_t np = 0;
        for (auto& n : mibs) {
            CHECK(np < npc);
            if (np >= npc) {
                break;
            }
            CHECK(n == std::string(mib_list[np++]));
        }
    }
    TEST_CASE("MIB: enable") {
        std::vector<std::string> mibs;
        xia::mib::mib_walk_func walker =
            [&mibs](xia::mib::node& nod) {
                mibs.push_back(nod.name());
            };
        const char* mib_list[] = {
            "a.1",
            "a.2",
            "b.1",
            "b.2.ro",
            "i.1",
            "i.2.ro",
            "r.1",
            "r.2.ro",
            "s.1",
            "s.2.ro",
            "t.1",
            "t.2.ro",
            "u.1",
            "u.2.ro",
        };
        for (auto n : mib_list) {
            CHECK(xia::mib::contains(n) == true);
        }
        const char* mib_disable_list[] = {
            "a.1",
            "a.2",
            "b.1",
            "i.1",
            "r.1",
            "s.1",
            "t.1",
            "u.1",
        };
        for (auto n : mib_disable_list) {
            CHECK_NOTHROW(xia::mib::disable(n));
        }
        for (auto n : mib_list) {
            CHECK(xia::mib::contains(n) == true);
        }
        mibs.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        const char* mib_enable_list[] = {
            "b.2.ro",
            "i.2.ro",
            "r.2.ro",
            "s.2.ro",
            "t.2.ro",
            "u.2.ro",
        };
        size_t npc = sizeof(mib_enable_list) / sizeof(mib_enable_list[0]);
        size_t np = 0;
        CHECK(npc == mibs.size());
        for (auto& n : mibs) {
            CHECK(np < npc);
            if (np >= npc) {
                break;
            }
            CHECK(n == std::string(mib_enable_list[np++]));
        }
        for (auto n : mib_disable_list) {
            CHECK(xia::mib::is_enabled(n) == false);
        }
        for (auto n : mib_enable_list) {
            CHECK(xia::mib::is_enabled(n) == true);
        }
        for (auto n : mib_disable_list) {
            auto nod = xia::mib::node(n);
            CHECK_NOTHROW(nod.enable());
        }
        mibs.clear();
        CHECK_NOTHROW(xia::mib::walk(walker));
        npc = sizeof(mib_list) / sizeof(mib_list[0]);
        np = 0;
        CHECK(npc == mibs.size());
        for (auto& n : mibs) {
            CHECK(np < npc);
            if (np >= npc) {
                break;
            }
            CHECK(n == std::string(mib_list[np++]));
        }
    }
    TEST_CASE("MIB: alphanumeric sort") {
        CHECK_NOTHROW(xia::mib::add_ro_str("2.2", "alphanum sort"));
        CHECK_NOTHROW(xia::mib::node("2.1", xia::mib::type::boolean));
        CHECK_NOTHROW(xia::mib::add_ro_int("1.1", 200));
        CHECK_NOTHROW(xia::mib::node("a.0", xia::mib::type::uinteger));
        CHECK_NOTHROW(xia::mib::add_ro_uint("a.a", 4000U));
        CHECK_NOTHROW(xia::mib::node("r.2", xia::mib::type::real));
        CHECK_NOTHROW(xia::mib::add_ro_real("r.3", 6.66666));
        std::vector<std::string> mibs;
        xia::mib::mib_walk_func walker =
            [&mibs](xia::mib::node& nod) {
                mibs.push_back(nod.name());
            };
        CHECK_NOTHROW(xia::mib::walk(walker));
        const char* mib_list[] = {
            "1.1",
            "2.1",
            "2.2",
            "a.0",
            "a.1",
            "a.2",
            "a.a",
            "b.1",
            "b.2.ro",
            "i.1",
            "i.2.ro",
            "r.1",
            "r.2",
            "r.2.ro",
            "r.3",
            "s.1",
            "s.2.ro",
            "t.1",
            "t.2.ro",
            "u.1",
            "u.2.ro"
        };
        const size_t npc = sizeof(mib_list) / sizeof(mib_list[0]);
        CHECK(npc == mibs.size());
        size_t np = 0;
        for (auto& n : mibs) {
            CHECK(np < npc);
            if (np >= npc) {
                break;
            }
            CHECK(n == std::string(mib_list[np++]));
        }
    }
    TEST_CASE("MIB: read/write") {
        size_t v = 0;
        auto rw1 = xia::mib::read_write("rw.1", v, xia::mib::rw_mode::rw);
        size_t r;
        CHECK_NOTHROW(r = *rw1);
        CHECK(r == 0);
        v = 1;
        CHECK_NOTHROW(r = *rw1);
        CHECK(r == 1);
        CHECK_NOTHROW(*rw1 = 2);
        CHECK(v == 2);
        std::mutex lock;
        std::string s = "locked ro mib read";
        auto ro1 = xia::mib::read_write_lock("rw.2", s, lock);
        CHECK_THROWS_WITH_AS(ro1.nod = "write locked set", "mib::node::set: MIB is write locked", xia::mib::error);
        CHECK(*ro1 == "locked ro mib read");
        std::atomic_int as1 = 100;
        auto rw3 = xia::mib::read_write("rw.3", as1, xia::mib::rw_mode::rw);
        CHECK(as1 == 100);
        CHECK(*rw3 == 100);
        *rw3 = 200;
        CHECK(as1 == 200);
        std::atomic_size_t as2 = 123;
        auto rw4 = xia::mib::read_write("rw.4", as2, xia::mib::rw_mode::rw);
        CHECK(as2 == 123);
        CHECK(*rw4 == 123);
        *rw4 = 321;
        CHECK(as2 == 321);
        std::atomic<double> as3 = 9876.54321;
        auto rw5 = xia::mib::read_write("rw.5", as3, xia::mib::rw_mode::rw);
        CHECK(as3 == 9876.54321);
        CHECK(*rw5 == 9876.54321);
        *rw5 = 54321.9876;
        CHECK(as3 == 54321.9876);
    }
    TEST_CASE("MIB: empty nodes") {
        xia::mib::node n;
        CHECK(!n.valid());
        CHECK_NOTHROW(n = xia::mib::node());
        CHECK(!n.valid());
        CHECK_NOTHROW(n = xia::mib::find("s.1"));
        CHECK(n.valid());
        CHECK(!n.read_only());
        CHECK_NOTHROW(n = "one fish, two fish, red fish, blue fish ...");
        CHECK(n == "one fish, two fish, red fish, blue fish ...");
        CHECK(!n.write_locked());
        CHECK_NOTHROW(n = xia::mib::find("s.2.ro"));
        CHECK(n == "s2 read-only");
        CHECK(n != "one fish, two fish, red fish, blue fish ...");
        CHECK(n.valid());
        CHECK(n.read_only());
        CHECK(n.write_locked());
    }
    TEST_CASE("MIB: remove nodes") {
        xia::mib::node n1;
        xia::mib::node n2;
        CHECK_NOTHROW(n1 = xia::mib::find("a.1"));
        CHECK_NOTHROW(n2 = n1);
        CHECK_THROWS_WITH_AS(n2.remove(), "mib::remove: mib base has references: a.1", xia::mib::error);
        CHECK(n2.valid() == true);
        CHECK_NOTHROW(n2.reset());
        CHECK(n2.valid() == false);
        CHECK_NOTHROW(n1.remove());
        CHECK(n1.valid() == false);
        CHECK_THROWS_WITH_AS(n2 = xia::mib::find("a.1"), "mib::find: mib not found: a.1", xia::mib::error);
    }
    TEST_CASE("MIB: set value") {
        xia::mib::node n;
        CHECK_NOTHROW(n = xia::mib::find("s.1"));
        CHECK_NOTHROW(n.set_value("string string"));
        CHECK(n == "string string");
        CHECK_NOTHROW(n = xia::mib::find("b.1"));
        CHECK_NOTHROW(n.set_value("false"));
        CHECK(n == false);
        CHECK_NOTHROW(n.set_value("0"));
        CHECK(n == false);
        CHECK_NOTHROW(n.set_value("1"));
        CHECK(n == true);
        CHECK_NOTHROW(n.set_value("true"));
        CHECK(n == true);
        CHECK_THROWS_WITH_AS(
                n.set_value("2345"),
                "mib::node: set: invalid value: b.1: 2345",
                xia::mib::error);
        CHECK_NOTHROW(n = xia::mib::find("i.1"));
        CHECK_NOTHROW(n.set_value("0"));
        CHECK(n == 0);
        CHECK_NOTHROW(n.set_value("100"));
        CHECK(n == 100);
        CHECK_NOTHROW(n.set_value("-200"));
        CHECK(n == -200);
        CHECK_NOTHROW(n.set_value("0x4000"));
        CHECK(n == 0x4000);
        CHECK_NOTHROW(n.set_value("0777"));
        CHECK(n == 0777);
        CHECK_THROWS_WITH_AS(
                n.set_value("0xffffffffffffffff"),
                "mib::node: set: invalid value: i.1: 0xffffffffffffffff",
                xia::mib::error);
        CHECK_THROWS_WITH_AS(
                n.set_value("abcd"),
                "mib::node: set: invalid value: i.1: abcd",
                xia::mib::error);
        CHECK_NOTHROW(n = xia::mib::find("u.1"));
        CHECK_NOTHROW(n.set_value("0"));
        CHECK(n == 0);
        CHECK_NOTHROW(n.set_value("100"));
        CHECK(n == 100);
        CHECK_NOTHROW(n.set_value("0x4000"));
        CHECK(n == 0x4000);
        CHECK_NOTHROW(n.set_value("0777"));
        CHECK(n == 0777);
        CHECK_NOTHROW(n.set_value("0xffffffffffffffff"));
        CHECK(n == 0xffffffffffffffffUL);
        CHECK_THROWS_WITH_AS(
                n.set_value("-200"),
                "mib::node: set: invalid value: u.1: -200",
                xia::mib::error);
        CHECK_THROWS_WITH_AS(
                n.set_value("abcd"),
                "mib::node: set: invalid value: u.1: abcd",
                xia::mib::error);
        CHECK_NOTHROW(n = xia::mib::find("r.1"));
        CHECK_NOTHROW(n.set_value("0"));
        CHECK(n == 0.0);
        CHECK_NOTHROW(n.set_value("100"));
        CHECK(n == 100.0);
        CHECK_NOTHROW(n.set_value("0.0001"));
        CHECK(n == 0.0001);
        CHECK_NOTHROW(n.set_value("-1.23456789"));
        CHECK(n == -1.23456789);
        CHECK_THROWS_WITH_AS(
                n.set_value("abcd"),
                "mib::node: set: invalid value: r.1: abcd",
                xia::mib::error);
        CHECK_NOTHROW(n = xia::mib::find("t.1"));
        CHECK_NOTHROW(n.set_value("2024-04-10T01:04:24.001386"));
        xia::mib::timestamp t(1712711064001386000);
        CHECK(n == t);
    }
}
