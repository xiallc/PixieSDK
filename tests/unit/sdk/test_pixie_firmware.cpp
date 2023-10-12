/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

/** @file test_pixie_firmware.cpp
 * @brief
 */

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/fw.hpp>

using fw_error = xia::pixie::error::error;

using firmware_set_def = std::vector<std::string>;
using firmware_set_defs = std::vector<firmware_set_def>;

static const firmware_set_defs fw_defs_good = {
    /* 15-250-12 custom */
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    /* 15-250-12 released 1.1.1 */
    {"release=1.1.1, version=r33339, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    /* 15-500-14 released 1.1.1 */
    {"release=1.1.1, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    /* 15-500-14 released 1.2.3 */
    {"release=1.2.3, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=1.2.3, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.var"},
    /* 15-500-14 released 1.1.4 */
    {"release=1.1.4, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.4, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    /* 17-500-14 released 2.2.2 */
    {"release=2.2.2, version=r133339, revision=17, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=2.2.2, version=r42081, revision=17, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.var"}
};

static const firmware_set_defs fw_defs_bad_1 = {
    {"revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_1_exc =
    "firmware version mandatory: revision=15, adc-msps=250, adc-bits=12, device=sys, mask=1, " \
    "file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin";

static const firmware_set_defs fw_defs_bad_2 = {
    {"version=r33330, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_2_exc =
    "firmware revision mandatory: version=r33330, adc-msps=250, adc-bits=12, device=sys, mask=1, " \
    "file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin";

static const firmware_set_defs fw_defs_bad_3 = {
    {"version=r33330, revision=15, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_3_exc =
    "firmware ADC MSPS mandatory: version=r33330, revision=15, adc-bits=12, device=sys, mask=1, " \
    "file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin";

static const firmware_set_defs fw_defs_bad_4 = {
    {"version=r33330, revision=15, adc-msps=250, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_4_exc =
    "firmware ADC bits mandatory: version=r33330, revision=15, adc-msps=250, device=sys, mask=1, " \
    "file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin";

static const firmware_set_defs fw_defs_bad_5 = {
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_5_exc =
    "firmware: check: invalid device: invalid";

static const firmware_set_defs fw_defs_bad_6 = {
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_6_exc =
    "firmware mask mandatory: version=r33330, revision=15, adc-msps=250, adc-bits=12, device=sys, " \
    "file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin";

static const firmware_set_defs fw_defs_bad_7 = {
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"}
};
static const char* fw_defs_bad_7_exc =
    "firmware file name mandatory: version=r33330, revision=15, adc-msps=250, adc-bits=12, device=sys, mask=1";

static const firmware_set_defs fw_defs_bad_8 = {
    {"version=r33330, revision=a, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"}
};
static const char* fw_defs_bad_8_exc =
    "firmware: module revision not a number: revision=a: a: invalid arg";

static const firmware_set_defs fw_defs_bad_9 = {
    {"version=r33330, revision=15, adc-msps=xx, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"}
};
static const char* fw_defs_bad_9_exc =
    "firmware: module ADC MSPS not a number: adc-msps=xx: xx: invalid arg";

static const firmware_set_defs fw_defs_bad_10 = {
    {"version=r33330, revision=15, adc-msps=15, adc-bits=yy, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"}
};
static const char* fw_defs_bad_10_exc =
    "firmware: module ADC BITS not a number: adc-bits=yy: yy: invalid arg";

static const firmware_set_defs fw_defs_bad_11 = {
    {"version=r33330, revision=15, adc-msps=15, adc-bits=12, " \
     "device=sys, mask=zz, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"}
};
static const char* fw_defs_bad_11_exc =
    "firmware: mask not a number: mask=zz: zz: invalid arg";

static void test_setup() {
    xia::logging::start("log", "test-firmware.txt", false);
    xia::logging::set_level(xia::log::level::info);
}

static void load_fw_sets(
    xia::pixie::firmware::system& firmwares, const firmware_set_defs& set_defs,
    xia::pixie::firmware::release_type release = xia::pixie::firmware::not_released) {
    for (auto& fw_set_def : set_defs) {
        xia::pixie::firmware::firmware_set fw_set(release, "release date");
        for (auto& fw_def : fw_set_def) {
            auto fw = xia::pixie::firmware::parse(release, fw_def, ',');
            if (release == xia::pixie::firmware::not_released && fw.release != release) {
                release = fw.release;
            }
            fw_set.add(fw);
        }
        fw_set.release = release;
        xia::pixie::firmware::add(firmwares, fw_set);
    }
}

TEST_SUITE("Firmware") {
    TEST_CASE("setup") {
        test_setup();
    }
    TEST_CASE("firmware sets") {
        using namespace xia::pixie;
        firmware::system firmwares;
        auto& fw_def = fw_defs_good.back();

        xia::pixie::firmware::release_type r222("2.2.2");
        xia::pixie::firmware::release_type fw_release;

        auto fw_sys = xia::pixie::firmware::parse(fw_release, fw_def[0], ',');
        CHECK(fw_release == r222);
        CHECK(fw_sys.release == r222);
        CHECK(fw_sys.tag == "17-500-14");
        CHECK(fw_sys.version == "r133339");
        CHECK(fw_sys.mask == 1);
        CHECK(fw_sys.instance == 0);
        CHECK(fw_sys.device.name == "sys");
        CHECK(fw_sys.device.mod_revision == 17);
        CHECK(fw_sys.device.mod_adc_msps == 500);
        CHECK(fw_sys.device.mod_adc_bits == 14);
        CHECK(fw_sys.filename == "firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin");
        CHECK(fw_sys.crc == 0);

        auto fw_fippi = xia::pixie::firmware::parse(fw_release, fw_def[1], ',');
        CHECK(fw_release == r222);
        CHECK(fw_fippi.release == r222);
        CHECK(fw_fippi.tag == "17-500-14");
        CHECK(fw_fippi.version == "r42081");
        CHECK(fw_fippi.mask == 0xf);
        CHECK(fw_fippi.instance == 0);
        CHECK(fw_fippi.device.name == "fippi");
        CHECK(fw_fippi.device.mod_revision == 17);
        CHECK(fw_fippi.device.mod_adc_msps == 500);
        CHECK(fw_fippi.device.mod_adc_bits == 14);
        CHECK(fw_fippi.filename == "firmware/fippixie16_revfgeneral_14b500m_r42081.bin");
        CHECK(fw_fippi.crc == 0);

        auto fw_dsp = xia::pixie::firmware::parse(fw_release, fw_def[2], ',');
        CHECK(fw_release == r222);
        CHECK(fw_dsp.release == r222);
        CHECK(fw_dsp.tag == "17-500-14");
        CHECK(fw_dsp.version == "r41847");
        CHECK(fw_dsp.mask == 1);
        CHECK(fw_dsp.instance == 0);
        CHECK(fw_dsp.device.name == "dsp");
        CHECK(fw_dsp.device.mod_revision == 17);
        CHECK(fw_dsp.device.mod_adc_msps == 500);
        CHECK(fw_dsp.device.mod_adc_bits == 14);
        CHECK(fw_dsp.filename == "dsp/Pixie16DSP_revfgeneral_14b500m_r41847.ldr");
        CHECK(fw_dsp.crc == 0);

        auto fw_var = xia::pixie::firmware::parse(fw_release, fw_def[3], ',');
        CHECK(fw_release == r222);
        CHECK(fw_var.release == r222);
        CHECK(fw_var.tag == "17-500-14");
        CHECK(fw_var.version == "r41847");
        CHECK(fw_var.mask == 1);
        CHECK(fw_var.instance == 0);
        CHECK(fw_var.device.name == "var");
        CHECK(fw_var.device.mod_revision == 17);
        CHECK(fw_var.device.mod_adc_msps == 500);
        CHECK(fw_var.device.mod_adc_bits == 14);
        CHECK(fw_var.filename == "dsp/Pixie16DSP_revfgeneral_14b500m_r41847.var");
        CHECK(fw_var.crc == 0);

        xia::pixie::firmware::firmware_set fw_set_empty;
        CHECK(fw_set_empty.empty() == true);
        fw_set_empty.release = r222;
        CHECK(fw_set_empty.empty() == false);
        CHECK_NOTHROW(fw_set_empty.clear());
        CHECK(fw_set_empty.empty() == true);
        fw_set_empty.release_date = "date";
        CHECK(fw_set_empty.empty() == false);
        CHECK_NOTHROW(fw_set_empty.clear());
        fw_set_empty.slot.push_back(1);
        CHECK(fw_set_empty.empty() == false);
        CHECK_NOTHROW(fw_set_empty.clear());
        CHECK_NOTHROW(fw_set_empty.add(fw_sys));
        CHECK(fw_set_empty.empty() == false);
        CHECK_NOTHROW(fw_set_empty.clear());
        CHECK(fw_set_empty.empty() == true);

        CHECK(fw_set_empty.check_slot(0) == true);
        CHECK(fw_set_empty.check_slot(1) == true);
        CHECK(fw_set_empty.check_slot(10) == true);
        fw_set_empty.slot.push_back(1);
        CHECK(fw_set_empty.check_slot(0) == true);
        CHECK(fw_set_empty.check_slot(1) == true);
        CHECK(fw_set_empty.check_slot(10) == false);
        fw_set_empty.slot.push_back(10);
        CHECK(fw_set_empty.check_slot(0) == true);
        CHECK(fw_set_empty.check_slot(1) == true);
        CHECK(fw_set_empty.check_slot(10) == true);
        CHECK_NOTHROW(fw_set_empty.clear());

        CHECK(fw_set_empty.size() == 0);
        CHECK(fw_set_empty.device_count() == 0);
        CHECK_NOTHROW(fw_set_empty.add(fw_sys));
        CHECK(fw_set_empty.size() == 1);
        CHECK(fw_set_empty.device_count() == 1);
        CHECK_NOTHROW(fw_set_empty.add(fw_fippi));
        CHECK(fw_set_empty.size() == 2);
        CHECK(fw_set_empty.device_count() == 2);
        CHECK_NOTHROW(fw_set_empty.add(fw_dsp));
        CHECK(fw_set_empty.size() == 3);
        CHECK(fw_set_empty.device_count() == 3);
        CHECK_NOTHROW(fw_set_empty.add(fw_var));
        CHECK(fw_set_empty.size() == 4);
        CHECK(fw_set_empty.device_count() == 4);
        CHECK_NOTHROW(fw_set_empty.clear());

        CHECK_THROWS_WITH_AS(
            fw_set_empty.get_devices(),
            "firmware: set: get-devices: no firmware sets found",
            fw_error);
        CHECK_NOTHROW(fw_set_empty.add(fw_sys));
        CHECK(
            fw_set_empty.get_devices() ==
            xia::pixie::firmware::device_types({"sys", "fippi", "dsp", "var"}));
        CHECK_NOTHROW(fw_set_empty.clear());

        CHECK_THROWS_WITH_AS(
            fw_set_empty.tag(),
            "firmware: set: tag: no firmware sets found",
            fw_error);
        CHECK_NOTHROW(fw_set_empty.add(fw_sys));
        CHECK(fw_set_empty.tag() == "17-500-14");
        CHECK_NOTHROW(fw_set_empty.clear());

        CHECK_THROWS_WITH_AS(
            fw_set_empty.mod_revision(),
            "firmware: set: mod-revision: no firmware sets found",
            fw_error);
        CHECK_NOTHROW(fw_set_empty.add(fw_sys));
        CHECK(fw_set_empty.mod_revision() == 17);
        CHECK_NOTHROW(fw_set_empty.clear());

        xia::pixie::firmware::firmware_set fw_set_r222(fw_release, "01-01-1970");
        CHECK(fw_set_r222.release == r222);
        CHECK(fw_set_r222.release_date == "01-01-1970");
        CHECK(fw_set_r222.type() == xia::pixie::firmware::firmware_set::set_type::undefined);
        CHECK(fw_set_r222.firmwares.empty() == true);
        CHECK(fw_set_r222.slot.empty() == true);
        CHECK(fw_set_r222.empty() == false);
        CHECK(fw_set_r222.valid() == false);
        CHECK(fw_set_r222.device_count() == 0);
        CHECK_THROWS_WITH_AS(
            fw_set_r222.check(),
            "firmware: set: check: release set with no firmware",
            fw_error);

        xia::pixie::firmware::firmware_set fw_set_r222_str("2.2.2", "01-01-1970");
        CHECK(fw_set_r222_str.release == r222);
        CHECK(fw_set_r222_str.release_date == "01-01-1970");
        CHECK((fw_set_r222 == fw_set_r222_str) == true);

        CHECK_NOTHROW(fw_set_r222.add(fw_sys));
        CHECK(fw_set_r222.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_r222.valid() == false);
        CHECK_NOTHROW(fw_set_r222.check());
        CHECK(fw_set_r222.tag() == "17-500-14");
        CHECK(fw_set_r222.mod_revision() == 17);
        CHECK(fw_set_r222.firmwares.size() == 1);
        CHECK(fw_set_r222.device_count() == 1);

        CHECK((fw_set_r222 == fw_set_r222_str) == false);

        CHECK_NOTHROW(fw_set_r222.add(fw_fippi));
        CHECK(fw_set_r222.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_r222.valid() == false);
        CHECK_NOTHROW(fw_set_r222.check());
        CHECK(fw_set_r222.firmwares.size() == 2);
        CHECK(fw_set_r222.device_count() == 2);

        CHECK_NOTHROW(fw_set_r222.add(fw_dsp));
        CHECK(fw_set_r222.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_r222.valid() == false);
        CHECK_NOTHROW(fw_set_r222.check());
        CHECK(fw_set_r222.firmwares.size() == 3);
        CHECK(fw_set_r222.device_count() == 3);

        CHECK_NOTHROW(fw_set_r222.add(fw_var));
        CHECK(fw_set_r222.type() == xia::pixie::firmware::firmware_set::set_type::release);
        CHECK(fw_set_r222.valid() == true);
        CHECK_NOTHROW(fw_set_r222.check());
        CHECK(fw_set_r222.firmwares.size() == 4);
        CHECK(fw_set_r222.device_count() == 4);

        fw_set_r222.slot.push_back(2);
        CHECK(fw_set_r222.slot == xia::pixie::firmware::firmware_set::slots({2}));

        xia::pixie::firmware::firmware_set fw_set_r222_copy(fw_set_r222);
        CHECK((fw_set_r222 == fw_set_r222_copy) == true);
        fw_set_r222_copy.slot[0] = 4;
        CHECK(fw_set_r222_copy.slot == xia::pixie::firmware::firmware_set::slots({4}));

        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_r222));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["17-500-14"].size() == 1);
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_r222_copy));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["17-500-14"].size() == 1);

        xia::pixie::firmware::firmware_set fw_set;
        xia::pixie::firmware::find_filter filter("17-500-14", "2.2.2");
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.type() == xia::pixie::firmware::firmware_set::set_type::release);
        CHECK(fw_set.valid() == true);
        CHECK_NOTHROW(fw_set.check());
        CHECK(fw_set.firmwares.size() == 4);
        /* check slots are merged */
        CHECK(fw_set.slot == xia::pixie::firmware::firmware_set::slots({2, 4}));

        /* Duplicate known device */
        xia::pixie::firmware::firmware_set fw_set_par;
        CHECK_NOTHROW(fw_set_par.add(fw_sys));
        CHECK_THROWS_WITH_AS(
            fw_set_par.add(fw_sys),
            "firmware: add: duplicate firmware: fw:17-500-14 device:sys",
            fw_error);
    }
    TEST_CASE("parse") {
        using namespace xia::pixie;
        firmware::system firmwares;
        CHECK_NOTHROW(load_fw_sets(firmwares, fw_defs_good));
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_1), fw_defs_bad_1_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_2), fw_defs_bad_2_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_3), fw_defs_bad_3_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_4), fw_defs_bad_4_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_5), fw_defs_bad_5_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_6), fw_defs_bad_6_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_7), fw_defs_bad_7_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_8), fw_defs_bad_8_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_9), fw_defs_bad_9_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_10), fw_defs_bad_10_exc, fw_error);
        CHECK_THROWS_WITH_AS(
            load_fw_sets(firmwares, fw_defs_bad_11), fw_defs_bad_11_exc, fw_error);
    }
    TEST_CASE("system firmwares") {
        using namespace xia::pixie;
        firmware::system firmwares;
        auto& fw_def = fw_defs_good.back();
        xia::pixie::firmware::release_type r222("2.2.2");
        xia::pixie::firmware::release_type r333("3.3.3");
        xia::pixie::firmware::release_type fw_release;
        auto fw_sys = xia::pixie::firmware::parse(fw_release, fw_def[0], ',');
        auto fw_fippi = xia::pixie::firmware::parse(fw_release, fw_def[1], ',');
        auto fw_dsp = xia::pixie::firmware::parse(fw_release, fw_def[2], ',');
        auto fw_var = xia::pixie::firmware::parse(fw_release, fw_def[3], ',');

        xia::pixie::firmware::firmware_set fw_set_par;
        CHECK_NOTHROW(fw_set_par.add(fw_sys));
        fw_set_par.firmwares[0]->filename = "fw_set_par";

        xia::pixie::firmware::firmware_set fw_set_cust;
        CHECK_NOTHROW(fw_set_cust.add(fw_sys));
        CHECK_NOTHROW(fw_set_cust.add(fw_fippi));
        CHECK_NOTHROW(fw_set_cust.add(fw_dsp));
        CHECK_NOTHROW(fw_set_cust.add(fw_var));
        fw_set_cust.firmwares[0]->filename = "fw_set_cust";

        xia::pixie::firmware::firmware_set fw_set_rel(r333, "01/01/1970");
        auto fw_rel_sys = xia::pixie::firmware::firmware(fw_sys.device, r333, fw_sys.version, fw_sys.mask);
        auto fw_rel_fippi = xia::pixie::firmware::firmware(fw_fippi.device, r333, fw_fippi.version, fw_fippi.mask);
        auto fw_rel_dsp = xia::pixie::firmware::firmware(fw_dsp.device, r333, fw_dsp.version, fw_dsp.mask);
        auto fw_rel_var = xia::pixie::firmware::firmware(fw_var.device, r333, fw_var.version, fw_var.mask);
        fw_rel_sys.filename = "fw_set_rel";
        fw_rel_fippi.filename = fw_fippi.filename;
        fw_rel_dsp.filename = fw_dsp.filename;
        fw_rel_var.filename = fw_var.filename;
        CHECK_NOTHROW(fw_set_rel.add(fw_rel_sys));
        CHECK_NOTHROW(fw_set_rel.add(fw_rel_fippi));
        CHECK_NOTHROW(fw_set_rel.add(fw_rel_dsp));
        CHECK_NOTHROW(fw_set_rel.add(fw_rel_var));

        firmwares.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));

        firmwares.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));

        firmwares.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));

        xia::pixie::firmware::firmware_set fw_set;
        xia::pixie::firmware::find_filter filter("17-500-14");

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_THROWS_WITH_AS(
            xia::pixie::firmware::find(fw_set, firmwares, filter),
            "firmware: find: firmware set not found: tag:17-500-14 release:0.0.0 slot:0",
            fw_error);
        CHECK(fw_set.valid() == false);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_cust");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r222);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::release_type("3.3.3"));
        CHECK(fw_set.firmwares[0]->filename == "fw_set_rel");
        CHECK(fw_set.firmwares[0]->release == r333);
        CHECK(fw_set.firmwares[1]->release == r333);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_par");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r222);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_par");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r222);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_par");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r333);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_cust");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r222);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        filter.set_release("3.3.3");
        filter.release_only = false;
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == xia::pixie::firmware::not_released);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_par");
        CHECK(fw_set.firmwares[0]->release == r222);
        CHECK(fw_set.firmwares[1]->release == r222);

        firmwares.clear();
        fw_set.clear();
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_par));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_cust));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rel));
        filter.set_release("3.3.3");
        filter.release_only = true;
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set, firmwares, filter));
        CHECK(fw_set.valid() == true);
        CHECK(fw_set.release == r333);
        CHECK(fw_set.firmwares[0]->filename == "fw_set_rel");
        CHECK(fw_set.firmwares[0]->release == r333);
        CHECK(fw_set.firmwares[1]->release == r333);
    }
}
