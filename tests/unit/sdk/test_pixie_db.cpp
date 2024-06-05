
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

/** @file test_pixie_db.cpp
 * @brief Provides test coverage for dbs.
 */

#include <algorithm>
#include <map>

#include <doctest/doctest.h>
#include <pixie/eeprom.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/pixie16/sim.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/db/module.hpp>
#include <pixie/pixie16/crate-view.hpp>

using eeprom_data = xia::pixie::sim::eeprom_data;
using eeprom_slot_def = xia::pixie::sim::eeprom_slot_def;

static const std::vector<std::string> mod_defs = {
    "device-number=0 slot=2 revision=15 eeprom-format=1 serial-num=1034 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=1 slot=6 revision=15 eeprom-format=1 serial-num=1035 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=2 slot=11 revision=17 eeprom-format=1 serial-num=2044 num-channels=32 "
    "adc-bits=12 adc-msps=500 adc-clk-div=5",
    "device-number=3 slot=12 revision=17 eeprom-format=1 serial-num=2043 num-channels=16 "
    "adc-bits=12 adc-msps=500 adc-clk-div=5",
    "device-number=4 slot=13 revision=12 eeprom-format=1 serial-num=1001 num-channels=16 "
    "adc-bits=12 adc-msps=100 adc-clk-div=1"};

static const xia::pixie::sim::firmware_set_defs firmware_defs = {
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.1.1, version=r33339, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.1.1, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.2.3, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=1.2.3, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_14b500m_r41847.var"},
    {"release=1.1.4, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.4, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=2.2.2, version=r133339, revision=17, adc-msps=250, adc-bits=14, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=2.2.2, version=r42081, revision=17, adc-msps=250, adc-bits=14, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_14b250m_r42081.bin",
     "release=2.2.2, version=r41847, revision=17, adc-msps=250, adc-bits=14, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_14b250m_r41847.ldr",
     "release=2.2.2, version=r41847, revision=17, adc-msps=250, adc-bits=14, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_14b250m_r41847.var"},
    {"release=2.2.2, version=r133339, revision=17, adc-msps=500, adc-bits=12, " \
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=2.2.2, version=r42081, revision=17, adc-msps=500, adc-bits=12, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_12b500m_r42081.bin",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=12, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_12b500m_r41847.ldr",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=12, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_12b500m_r41847.var"},
    {"release=0.9.1, version=r30882, revision=12, adc-msps=100, adc-bits=12, " \
     "device=sys, mask=1, file=syspixie16_revdgeneral_adc100mhz_r30882.bin",
     "release=0.9.1, version=r30944, revision=12, adc-msps=100, adc-bits=12, " \
     "device=fippi, mask=0xf, file=fippixie16_revdgeneral_12b100m_r30944.bin",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=dsp, mask=1, file=Pixie16DSP_revdgeneral_12b100m_r30918.ldr",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=var, mask=1, file=Pixie16DSP_revdgeneral_12b100m_r30918.var"}
};

static const eeprom_data rev_h_db10 = {
    "00000000 17 29 60 3e f2 ff 0b fb-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 0a 01 28 0a-02 28 0a 03 28 0a 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_data rev_h_db04 = {
    "00000000 12 f2 6d 63 f2 ff 0b fc-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 01 28 04-02 28 04 03 28 04 04 ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_slot_def eep_defs = {{11,12}, {rev_h_db04, rev_h_db10}};

static void test_setup() {
    xia::logging::start("log", "test-db.txt", false);
    xia::logging::set_level(xia::log::level::info);
    xia::pixie::sim::clear_module_defs();
    xia::pixie::sim::clear_eeprom_defs();
    for (auto& def : mod_defs) {
        xia::pixie::sim::add_module_def(def);
    }
    xia::pixie::sim::load_eeprom_defs(eep_defs);
}

TEST_SUITE("xia::pixie::fixture::db") {
    TEST_CASE("DB10") {
        SUBCASE("Setup") {
            test_setup();
        }
        SUBCASE("Boot and Offline") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK(crate.num_modules == 5);
            CHECK(crate[3].fixtures->subassemblies.size() == 4);
            CHECK_NOTHROW(crate->boot());
            CHECK_NOTHROW(crate->set_offline(12));
        }
        SUBCASE("Test modes") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            for (size_t chan_num = 0; chan_num < crate[3].num_channels; chan_num++) {
                auto& channel = crate[3].channels[chan_num];
                size_t db_number;
                channel.fixture->get("db.number", db_number);
                CHECK(db_number == (chan_num/4));
            }
            CHECK(crate[3].fixtures->subassemblies.size() == 4);
            bool hastest;
            crate[3].channels[0].fixture->get("adc.has.self-test", hastest);
            CHECK(hastest == true);
            crate[3].channels[0].fixture->get("adc.has.test-mode", hastest);
            CHECK(hastest == true);
            auto node = xia::mib::find("module.12.db10.0.channel.0.adc.ad9434");
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:off");
            auto mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::fs_plus);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:+FS");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::fs_minus);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:-FS");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::midscale);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:midscale");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::checkerboard);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:checkerboard");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::pn23);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:pn23");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::pn9);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:pn9");
            mode = static_cast<int>(xia::pixie::fixture::db::channel::adc_test_mode::one_zero);
            crate[3].channels[0].fixture->set("adc.test-mode", mode);
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:one/zero");
            node = xia::mib::find("module.12.db10.0.channel.1.adc.ad9434");
            CHECK(node == "coupling:dc,fmt:offset-binary,invert:off,test-mode:off");
            CHECK_THROWS_WITH_AS(crate[3].channels[0].fixture->set("adc.test-mode", 12392935),
                "module: num=3,slot=12: invalid ADC test mode", xia::pixie::module::error);
        }
        SUBCASE("Bitslip") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            bool hasbitslip;
            crate[3].channels[0].fixture->get("adc.has.bitslip", hasbitslip);
            CHECK(hasbitslip == true);
            crate[3].channels[0].fixture->set("adc.bitslip", 4);
            auto node = xia::mib::find("module.12.db10.0.channel.0.bitslip");
            CHECK(node == 4);
            crate[3].channels[3].fixture->set("adc.bitslip", 7);
            node = xia::mib::find("module.12.db10.0.channel.3.bitslip");
            CHECK(node == 7);
            node = xia::mib::find("module.12.db10.0.channel.0.bitslip");
            CHECK(node == 4);
            unsigned int value;
            crate[3].channels[0].fixture->get("adc.bitslip.bits", value);
            CHECK(value == 16);
        }
        SUBCASE("DB10 Specs") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            bool has;
            unsigned int value;
            crate[3].channels[0].fixture->get("adc.default.inverted", has);
            CHECK(has == true);
            crate[3].channels[0].fixture->get("adc.test-mode.result.pass", has);
            CHECK(has == false);
            crate[3].channels[0].fixture->get("adc.swap.disable", has);
            CHECK(has == true);
            crate[3].channels[0].fixture->get("dac.settle-period", value);
            CHECK(value == 100);
            CHECK_THROWS_WITH_AS(crate[3].channels[0].fixture->get("fake-value", value),
                "invalid fixture op: no get support: unsigned int: fake-value", xia::pixie::error::error);
            crate[3].channels[0].fixture->get("fake-value", has);
            CHECK(has == false);
        }
        SUBCASE("Set DAC") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            CHECK_NOTHROW(crate[3].set_dacs());
        }
        SUBCASE("Cleanup") {
            xia::pixie::sim::clear_module_defs();
            xia::pixie::sim::clear_eeprom_defs();
        }
    }
    TEST_CASE("DB04") {
        SUBCASE("Setup") {
            test_setup();
        }
        SUBCASE("Boot and Offline") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK(crate.num_modules == 5);
            CHECK(crate[2].fixtures->subassemblies.size() == 4);
            CHECK_NOTHROW(crate->boot());
            CHECK_NOTHROW(crate->set_offline(11));
        }
        SUBCASE("DB04 Specs") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            bool has;
            unsigned int value;
            crate[2].channels[0].fixture->get("dac.has.offset", has);
            CHECK(has == true);
            crate[2].channels[0].fixture->get("dac.settle-period", value);
            CHECK(value == 250);
            CHECK_THROWS_WITH_AS(crate[2].channels[0].fixture->get("fake-value", value),
                "invalid fixture op: no get support: unsigned int: fake-value", xia::pixie::error::error);
            crate[2].channels[0].fixture->get("fake-value", has);
            CHECK(has == false);
        }
        SUBCASE("Set DAC") {
            xia::pixie::sim::crate sim_crate;
            xia::pixie::sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
            xia::pixie::crate::view::module crate(sim_crate);
            CHECK_NOTHROW(crate->initialize());
            CHECK_NOTHROW(crate->boot());
            CHECK_NOTHROW(crate[2].set_dacs());
        }
        SUBCASE("Cleanup") {
            xia::pixie::sim::clear_module_defs();
            xia::pixie::sim::clear_eeprom_defs();
        }
    }
}
