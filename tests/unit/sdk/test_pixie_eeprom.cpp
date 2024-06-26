/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
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

/** @file test_pixie_epprom.cpp
 * @brief Provides test coverage for eeprom namespace.
 */

#include <algorithm>
#include <map>

#include <doctest/doctest.h>
#include <pixie/eeprom.hpp>
#include <pixie/utils/string.hpp>

using eeprom_error = xia::pixie::eeprom::error;

namespace eetest {
using eeprom_addr = unsigned long;
using eeprom_data = std::vector<std::string>;
using rev_eeprom_data = std::map<std::string, const eeprom_data&>;

/*
 * Lets handle the EEPROM data as found the log. The less we touch it the
 * better the results we get in this unit test.
 */
static const eeprom_data blank = {
    "00000000 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000010 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_data rev_d = {
    "00000000 ac 0c 00 28 01 00 09 02-00 28 01 00 09 02 00 28 ................",
    "00000010 01 00 09 02 00 28 01 00-09 02 00 28 01 00 09 02 ................",
    "00000020 00 28 01 00 09 02 00 28-01 00 09 02 00 28 01 00 ................",
    "00000030 09 02 00 28 01 00 09 02-00 28 01 00 09 02 00 28 ................",
    "00000040 01 00 09 02 00 28 01 00-09 02 00 28 01 00 09 02 ................",
    "00000050 00 28 01 00 09 02 00 28-01 00 09 02 00 28 01 00 ................",
    "00000060 09 02 ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_f = {
    "00000000 d8 04 0f 00 19 01 00 06-02 00 19 01 00 06 02 00 ................",
    "00000010 19 01 00 06 02 00 19 01-00 06 02 00 19 01 00 06 ................",
    "00000020 02 00 19 01 00 06 02 00-19 01 00 06 02 00 19 01 ................",
    "00000030 00 06 02 00 19 01 00 06-02 00 19 01 00 06 02 00 ................",
    "00000040 19 01 00 06 02 00 19 01-00 06 02 00 19 01 00 06 ................",
    "00000050 02 00 19 01 00 06 02 00-19 01 00 06 02 00 19 01 ................",
    "00000060 00 06 02 10 fa 00 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_data rev_h_db04 = {
    "00000000 b4 f0 42 94 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 01 28 04-02 28 04 03 28 04 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db01_75 = {
    "00000000 3d ea f3 22 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 00 01 28 00-02 28 00 03 28 00 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db01_125 = {
    "00000000 0f ed 83 79 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 01 01 28 01-02 28 01 03 28 01 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db02 = {
    "00000000 59 e4 13 94 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 02 01 28 02-02 28 02 03 28 02 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db05 = {
    "00000000 86 f7 32 cf f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 05 01 28 05-02 28 05 03 28 05 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db06 = {
    "00000000 d0 fe a2 22 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 06 01 28 06-02 28 06 03 28 06 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db07 = {
    "00000000 e2 f9 d2 79 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 07 01 28 07-02 28 07 03 28 07 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_few_dbs = {
    "00000000 5b 9c f5 a5 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 01 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_mixed_dbs = {
    "00000000 73 d1 90 bf f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 01 28 05-02 28 04 03 28 05 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_bad_crc = {
    "00000000 61 96 ec e3 f2 ff 4b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 00 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_db10 = {
    "00000000 6d d7 f4 fe f2 ff 0b de-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 0a 01 28 0a-02 28 0a 03 28 0a 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const rev_eeprom_data eeproms = {
    { "blank", blank },
    { "D", rev_d },
    { "F", rev_f },
    { "H:DB04", rev_h_db04 },
    { "H:DB01-75", rev_h_db01_75 },
    { "H:DB01-125", rev_h_db01_125 },
    { "H:DB02", rev_h_db02 },
    { "H:DB05", rev_h_db05 },
    { "H:DB06", rev_h_db06 },
    { "H:DB07", rev_h_db07 },
    { "H:few-dbs", rev_h_few_dbs },
    { "H:mixed-dbs", rev_h_mixed_dbs },
    { "H:bad-crc", rev_h_bad_crc },
    { "H:DB10", rev_h_db10 },
};

template<typename T>
static T get_value(const std::string& opt, int base = 9) {
    T value = 0;
    try {
        value = T(std::stoul(opt, nullptr, base));
    } catch (std::invalid_argument& ) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range& ) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

static void load(const std::string rev, xia::pixie::eeprom::eeprom& eeprom) {
    auto edi = eeproms.find(rev);
    if (edi != std::end(eeproms)) {
        eeprom.clear();
        eeprom.data.resize(xia::pixie::hw::eeprom_block_size);
        for (auto& s : std::get<1>(*edi)) {
            xia::util::string::strings ss;
            xia::util::string::split(ss, s);
            if (ss.size() != 17) {
                throw std::runtime_error("invalid EEPROM format: rev=" + rev);
            }
            eeprom_addr a = get_value<eeprom_addr>(ss[0], 16);
            if ((a + 16) > eeprom.data.size()) {
                throw std::runtime_error("invalid EEPROM address: " + ss[0]);
            }
            for (int b = 0; b < 7; ++b) {
                eeprom.data[a + b] = get_value<uint8_t>(ss[b + 1], 16);
            }
            xia::util::string::strings ss7_8;
            xia::util::string::split(ss7_8, ss[8], '-');
            if (ss7_8.size() != 2) {
                throw std::runtime_error("invalid EEPROM format: rev=" + rev);
            }
            eeprom.data[a + 7] = get_value<uint8_t>(ss7_8[0], 16);
            eeprom.data[a + 8] = get_value<uint8_t>(ss7_8[1], 16);
            for (int b = 9; b < 16; ++b) {
                eeprom.data[a + b] = get_value<uint8_t>(ss[b], 16);
            }
        }
    }
}
} // namespace eetest

TEST_SUITE("xia::pixie::eeprom") {
    xia::pixie::eeprom::eeprom eeprom;
    TEST_CASE("Blank") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("blank", eeprom));
        }
        SUBCASE("Process") {
            CHECK_THROWS_WITH_AS(
                eeprom.process(), "invalid serial number: blank EEPROM", eeprom_error);
        }
    }
    TEST_CASE("Rev D") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("D", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 0);
            CHECK(eeprom.valid() == false);
            CHECK(eeprom.serial_num == 172);
            CHECK(eeprom.revision == 12);
            CHECK(eeprom.major_revision == 0);
            CHECK(eeprom.minor_revision == 0);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 16);
            CHECK(eeprom.max_channels == 16);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 0);
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 16);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::mainboard);
                CHECK(cfg.adc_bits == 12);
                CHECK(cfg.adc_msps == 100);
                CHECK(cfg.adc_clk_div == 1);
                CHECK(cfg.fpga_clk_mhz == 100);
                CHECK(cfg.max_histogram_length == 32768);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 3640);
                ++index;
            }
        }
    }
    TEST_CASE("Rev F") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("F", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 1);
            CHECK(eeprom.valid() == false);
            CHECK(eeprom.serial_num == 1240);
            CHECK(eeprom.revision == 15);
            CHECK(eeprom.major_revision == 0);
            CHECK(eeprom.minor_revision == 0);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 16);
            CHECK(eeprom.max_channels == 16);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 0);
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 16);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::mainboard);
                CHECK(cfg.adc_bits == 16);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == 125);
                CHECK(cfg.max_histogram_length == 32768);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 3640);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB04") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB04", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 32);
            CHECK(eeprom.max_channels == 32);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB04");
                CHECK(eeprom.dbs[i].prom_id == 4);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 32);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB04);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB01 - 75 msps") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB01-75", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB01");
                CHECK(eeprom.dbs[i].prom_id == 0);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB01);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 75);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB01 - 125 msps") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB01-125", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB01");
                CHECK(eeprom.dbs[i].prom_id == 1);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB01);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 125);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB02") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB02", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB02");
                CHECK(eeprom.dbs[i].prom_id == 2);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB02);
                CHECK(cfg.adc_bits == 12);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB05") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB05", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB05");
                CHECK(eeprom.dbs[i].prom_id == 5);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB05);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 125);
                CHECK(cfg.adc_clk_div == 1);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB06") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB06", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB06");
                CHECK(eeprom.dbs[i].prom_id == 6);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB06);
                CHECK(cfg.adc_bits == 16);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB07") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB07", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB07");
                CHECK(eeprom.dbs[i].prom_id == 7);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB07);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 500);
                CHECK(cfg.adc_clk_div == 5);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - DB10") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:DB10", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2014);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 4);
            for (size_t i = 0; i < eeprom.dbs.size(); i++) {
                CHECK(eeprom.dbs[i].label == "DB10");
                CHECK(eeprom.dbs[i].prom_id == 10);
                CHECK(eeprom.dbs[i].position == (i + 1));
                CHECK(eeprom.dbs[i].index == i);
            }
        }
        SUBCASE("Configs") {
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB10);
                CHECK(cfg.adc_bits == 12);
                CHECK(cfg.adc_msps == 500);
                CHECK(cfg.adc_clk_div == 4);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - too few DBs") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:few-dbs", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 1);
            for (auto& db : eeprom.dbs) {
                CHECK(db.label == "DB04");
                CHECK(db.prom_id == 4);
                CHECK(db.position == 1);
            }
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 8);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB04);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == cfg.adc_msps / cfg.adc_clk_div);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - mixed DBs") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:mixed-dbs", eeprom));
        }
        SUBCASE("Process") {
            CHECK_THROWS_WITH_AS(eeprom.process(), "daughter board configuration error: mixture of DBs",
                                 eeprom_error);
        }
    }
    TEST_CASE("Rev H - bad crc") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:bad-crc", eeprom));
        }
        SUBCASE("Process") {
            CHECK_THROWS_WITH_AS(
                eeprom.process(), "invalid ADC MSPS: 65535", eeprom_error);
        }
    }
}
