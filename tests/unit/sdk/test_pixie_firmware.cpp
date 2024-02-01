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
#include <vector>

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
    /* 13-100-12 released 0.9.1 */
    {"release=0.9.1, version=r30882, revision=13, adc-msps=100, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revdgeneral_adc100mhz_r30882.bin",
     "release=0.9.1, version=r30944, revision=13, adc-msps=100, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revdgeneral_12b100m_r30944.bin",
     "release=0.9.1, version=r30918, revision=13, adc-msps=100, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.ldr",
     "release=0.9.1, version=r30918, revision=13, adc-msps=100, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.var"},
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

static const firmware_set_defs fw_defs_custom = {
    /* 15-250-12 custom 2 */
    {"version=r33339, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33338.bin",
     "version=r42081, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42080.bin",
     "version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41848.ldr",
     "version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41848.var"},
    /* 17-500-14 released 2.2.2 with bad sys mask */
    {"release=2.2.2, version=r133339, revision=17, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=2, file=firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin",
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

static const firmware_set_defs fw_defs_rev_d = {
    {"release=0.9.1, version=r30882, revision=13, adc-msps=100, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revdgeneral_adc100mhz_r30882.bin",
     "release=0.9.1, version=r30944, revision=13, adc-msps=100, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revdgeneral_12b100m_r30944.bin",
     "release=0.9.1, version=r30918, revision=13, adc-msps=100, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.ldr",
     "release=0.9.1, version=r30918, revision=13, adc-msps=100, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.var"}
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
        auto& fw_def_user = fw_defs_good.front();
        auto& fw_def_user_b = fw_defs_custom.front();
        auto& fw_def_bad_sys_mask = fw_defs_custom.back();
        auto& fw_def_rev_d = fw_defs_rev_d.front();

        xia::pixie::firmware::release_type r222("2.2.2");
        xia::pixie::firmware::release_type rCustom(xia::pixie::firmware::not_released);
        xia::pixie::firmware::release_type fw_release;
        xia::pixie::firmware::release_type fw_custom;

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

        /* firmware_set::empty */
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

        /* firmware_set::check_slot */
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

        /* Filling a firmware set */
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
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_empty));
        CHECK_NOTHROW(fw_set_empty.clear());
        CHECK_NOTHROW(firmwares.clear());

        /* Partial/User firmware tests */
        auto fw_sys_user = xia::pixie::firmware::parse(fw_custom, fw_def_user[0], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_sys_user.release == rCustom);
        CHECK(fw_sys_user.tag == "15-250-12");
        CHECK(fw_sys_user.version == "r33330");
        CHECK(fw_sys_user.mask == 1);
        CHECK(fw_sys_user.instance == 0);
        CHECK(fw_sys_user.device.name == "sys");
        CHECK(fw_sys_user.device.mod_revision == 15);
        CHECK(fw_sys_user.device.mod_adc_msps == 250);
        CHECK(fw_sys_user.device.mod_adc_bits == 12);
        CHECK(fw_sys_user.filename == "firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin");
        CHECK(fw_sys_user.crc == 0);

        auto fw_fippi_user = xia::pixie::firmware::parse(fw_custom, fw_def_user[1], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_fippi_user.release == rCustom);
        CHECK(fw_fippi_user.tag == "15-250-12");
        CHECK(fw_fippi_user.version == "r42070");
        CHECK(fw_fippi_user.mask == 0xf);
        CHECK(fw_fippi_user.instance == 0);
        CHECK(fw_fippi_user.device.name == "fippi");
        CHECK(fw_fippi_user.device.mod_revision == 15);
        CHECK(fw_fippi_user.device.mod_adc_msps == 250);
        CHECK(fw_fippi_user.device.mod_adc_bits == 12);
        CHECK(fw_fippi_user.filename == "firmware/fippixie16_revfgeneral_12b250m_r42081.bin");
        CHECK(fw_fippi_user.crc == 0);

        auto fw_dsp_user = xia::pixie::firmware::parse(fw_custom, fw_def_user[2], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_dsp_user.release == rCustom);
        CHECK(fw_dsp_user.tag == "15-250-12");
        CHECK(fw_dsp_user.version == "r41807");
        CHECK(fw_dsp_user.mask == 1);
        CHECK(fw_dsp_user.instance == 0);
        CHECK(fw_dsp_user.device.name == "dsp");
        CHECK(fw_dsp_user.device.mod_revision == 15);
        CHECK(fw_dsp_user.device.mod_adc_msps == 250);
        CHECK(fw_dsp_user.device.mod_adc_bits == 12);
        CHECK(fw_dsp_user.filename == "dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr");
        CHECK(fw_dsp_user.crc == 0);

        auto fw_var_user = xia::pixie::firmware::parse(fw_custom, fw_def_user[3], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_var_user.release == rCustom);
        CHECK(fw_var_user.tag == "15-250-12");
        CHECK(fw_var_user.version == "r41840");
        CHECK(fw_var_user.mask == 1);
        CHECK(fw_var_user.instance == 0);
        CHECK(fw_var_user.device.name == "var");
        CHECK(fw_var_user.device.mod_revision == 15);
        CHECK(fw_var_user.device.mod_adc_msps == 250);
        CHECK(fw_var_user.device.mod_adc_bits == 12);
        CHECK(fw_var_user.filename == "dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var");
        CHECK(fw_var_user.crc == 0);

        auto fw_sys_user_b = xia::pixie::firmware::parse(fw_custom, fw_def_user_b[0], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_sys_user_b.release == rCustom);
        CHECK(fw_sys_user_b.tag == "15-250-12");
        CHECK(fw_sys_user_b.version == "r33339");
        CHECK(fw_sys_user_b.device.name == "sys");
        CHECK(fw_sys_user_b.filename == "firmware/syspixie16_revfgeneral_adc250mhz_r33338.bin");

        auto fw_fippi_user_b = xia::pixie::firmware::parse(fw_custom, fw_def_user_b[1], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_fippi_user_b.release == rCustom);
        CHECK(fw_fippi_user_b.tag == "15-250-12");
        CHECK(fw_fippi_user_b.version == "r42081");
        CHECK(fw_fippi_user_b.device.name == "fippi");
        CHECK(fw_fippi_user_b.filename == "firmware/fippixie16_revfgeneral_12b250m_r42080.bin");

        auto fw_dsp_user_b = xia::pixie::firmware::parse(fw_custom, fw_def_user_b[2], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_dsp_user_b.release == rCustom);
        CHECK(fw_dsp_user_b.tag == "15-250-12");
        CHECK(fw_dsp_user_b.version == "r41847");
        CHECK(fw_dsp_user_b.device.name == "dsp");
        CHECK(fw_dsp_user_b.filename == "dsp/Pixie16DSP_revfgeneral_12b250m_r41848.ldr");

        auto fw_var_user_b = xia::pixie::firmware::parse(fw_custom, fw_def_user_b[3], ',');
        CHECK(fw_custom == rCustom);
        CHECK(fw_var_user_b.release == rCustom);
        CHECK(fw_var_user_b.tag == "15-250-12");
        CHECK(fw_var_user_b.version == "r41847");
        CHECK(fw_var_user_b.device.name == "var");
        CHECK(fw_var_user_b.filename == "dsp/Pixie16DSP_revfgeneral_12b250m_r41848.var");

        /* Filling a user firmware set */
        xia::pixie::firmware::firmware_set fw_set_user;
        CHECK(fw_set_user.size() == 0);
        CHECK(fw_set_user.device_count() == 0);
        CHECK_NOTHROW(fw_set_user.add(fw_sys_user));
        CHECK(fw_set_user.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_user.size() == 1);
        CHECK(fw_set_user.device_count() == 1);
        CHECK_NOTHROW(fw_set_user.add(fw_fippi_user));
        CHECK(fw_set_user.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_user.size() == 2);
        CHECK(fw_set_user.device_count() == 2);
        CHECK_NOTHROW(fw_set_user.add(fw_dsp_user));
        CHECK(fw_set_user.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK(fw_set_user.size() == 3);
        CHECK(fw_set_user.device_count() == 3);
        CHECK_NOTHROW(fw_set_user.add(fw_var_user));
        CHECK(fw_set_user.type() == xia::pixie::firmware::firmware_set::set_type::user);
        CHECK(fw_set_user.size() == 4);
        CHECK(fw_set_user.device_count() == 4);
        fw_set_user.slot.push_back(2);
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 1);

        /* Duplicate user set */
        xia::pixie::firmware::firmware_set fw_set_user_dup;
        CHECK_NOTHROW(fw_set_user_dup.add(fw_sys_user));
        CHECK(fw_set_user_dup.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dup.add(fw_fippi_user));
        CHECK(fw_set_user_dup.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dup.add(fw_dsp_user));
        CHECK(fw_set_user_dup.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dup.add(fw_var_user));
        CHECK(fw_set_user_dup.type() == xia::pixie::firmware::firmware_set::set_type::user);
        fw_set_user_dup.slot.push_back(3);
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_dup));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 1);
        xia::pixie::firmware::firmware_set fw_set_user_slots;
        xia::pixie::firmware::find_filter user_filter("15-250-12", rCustom);
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set_user_slots, firmwares, user_filter));
        CHECK(fw_set_user_slots.slot == xia::pixie::firmware::firmware_set::slots({2, 3}));
        CHECK_NOTHROW(fw_set_user_dup.clear());
        CHECK_NOTHROW(fw_set_user_slots.clear());

        /* Add different user set, same module tag */
        xia::pixie::firmware::firmware_set fw_set_user_dif;
        CHECK_NOTHROW(fw_set_user_dif.add(fw_sys_user_b));
        CHECK(fw_set_user_dif.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dif.add(fw_fippi_user_b));
        CHECK(fw_set_user_dif.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dif.add(fw_dsp_user_b));
        CHECK(fw_set_user_dif.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_user_dif.add(fw_var_user_b));
        CHECK(fw_set_user_dif.type() == xia::pixie::firmware::firmware_set::set_type::user);
        CHECK_THROWS_WITH_AS(xia::pixie::firmware::add(firmwares, fw_set_user_dif),
                             "firmware: set: add: different user set already exists: module:15-250-12",
                             fw_error);
        CHECK_NOTHROW(fw_set_user_dif.clear());

        /* Duplicate user set w/ partial firmware */
        xia::pixie::firmware::firmware_set fw_set_user_sys_dup;
        CHECK_NOTHROW(fw_set_user_sys_dup.add(fw_sys_user));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_sys_dup));
        fw_set_user_sys_dup.slot.push_back(3);
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);

        /* Add firmware with same device and different filename */
        xia::pixie::firmware::firmware_set fw_set_user_sys_bad_filename;
        CHECK_NOTHROW(fw_set_user_sys_bad_filename.add(fw_sys_user_b));
        CHECK_THROWS_WITH_AS(xia::pixie::firmware::add(firmwares, fw_set_user_sys_bad_filename),
                            "firmware: set: merge: file name changed: device:sys file:"
                            "firmware/syspixie16_revfgeneral_adc250mhz_r33338.bin", fw_error);
        CHECK_NOTHROW(fw_set_user_sys_bad_filename.clear());

        /* Duplicate user set w/ partial firmware continued */
        xia::pixie::firmware::firmware_set fw_set_user_fippi_dup;
        CHECK_NOTHROW(fw_set_user_fippi_dup.add(fw_fippi_user));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_fippi_dup));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);
        xia::pixie::firmware::firmware_set fw_set_user_dsp_dup;
        CHECK_NOTHROW(fw_set_user_dsp_dup.add(fw_dsp_user));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_dsp_dup));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);
        xia::pixie::firmware::firmware_set fw_set_user_var_dup;
        CHECK_NOTHROW(fw_set_user_var_dup.add(fw_var_user));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_var_dup));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 1);
        CHECK_NOTHROW(xia::pixie::firmware::find(fw_set_user_slots, firmwares, user_filter));
        CHECK(fw_set_user_slots.slot == xia::pixie::firmware::firmware_set::slots({2, 3}));
        CHECK_NOTHROW(fw_set_user_sys_dup.clear());
        CHECK_NOTHROW(fw_set_user_fippi_dup.clear());
        CHECK_NOTHROW(fw_set_user_dsp_dup.clear());
        CHECK_NOTHROW(fw_set_user_var_dup.clear());

        /* Add different user set w/ partial firmware, same module tag */
        xia::pixie::firmware::firmware_set fw_set_user_sys_dif;
        CHECK_NOTHROW(fw_set_user_sys_dif.add(fw_sys_user_b));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_sys_dif));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);
        xia::pixie::firmware::firmware_set fw_set_user_fippi_dif;
        CHECK_NOTHROW(fw_set_user_fippi_dif.add(fw_fippi_user_b));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_fippi_dif));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);
        xia::pixie::firmware::firmware_set fw_set_user_dsp_dif;
        CHECK_NOTHROW(fw_set_user_dsp_dif.add(fw_dsp_user_b));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_user_dsp_dif));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["15-250-12"].size() == 2);
        xia::pixie::firmware::firmware_set fw_set_user_var_dif;
        CHECK_NOTHROW(fw_set_user_var_dif.add(fw_var_user_b));
        CHECK_THROWS_WITH_AS(xia::pixie::firmware::add(firmwares, fw_set_user_var_dif),
                             "firmware: set: add: partial set creates user set and a different "
                             "user set already exists: module:15-250-12", fw_error);
        CHECK_NOTHROW(fw_set_user_sys_dif.clear());
        CHECK_NOTHROW(fw_set_user_fippi_dif.clear());
        CHECK_NOTHROW(fw_set_user_dsp_dif.clear());
        CHECK_NOTHROW(fw_set_user_var_dif.clear());
        CHECK_NOTHROW(firmwares.clear());

        /* firmware set info checks */
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

        /* Release firmware set tests */
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
        CHECK_THROWS_WITH_AS(
            xia::pixie::firmware::add(firmwares, fw_set_r222),
            "firmware: set: add: released set not complete: module:17-500-14 release:2.2.2",
            fw_error);

        CHECK((fw_set_r222 == fw_set_r222_str) == false);

        /* Filling a release firmware set */
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
        /* check firmware set was copied correctly */
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

        /* Adding different release firmware set, same module tag */
        fw_set_r222_copy.release_date = "01-01-1971";
        CHECK_THROWS_WITH_AS(xia::pixie::firmware::add(firmwares, fw_set_r222_copy),
                             "firmware: set: add: released set already exists: module:17-500-14"
                             " release:2.2.2", fw_error);

        /* Can not merge firmware with differing masks */
        xia::pixie::firmware::firmware_set fw_set_bad_sys_mask("2.2.2", "01-01-1970");
        auto fw_sys_bad_mask = xia::pixie::firmware::parse(fw_release, fw_def_bad_sys_mask[0], ',');
        CHECK(fw_sys_bad_mask.release == r222);
        CHECK(fw_sys_bad_mask.tag == "17-500-14");
        CHECK(fw_sys_bad_mask.mask == 2);
        CHECK(fw_sys_bad_mask.device.name == "sys");
        CHECK_NOTHROW(fw_set_bad_sys_mask.add(fw_sys_bad_mask));
        CHECK(fw_set_bad_sys_mask.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_bad_sys_mask.add(fw_fippi));
        CHECK(fw_set_bad_sys_mask.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_bad_sys_mask.add(fw_dsp));
        CHECK(fw_set_bad_sys_mask.type() == xia::pixie::firmware::firmware_set::set_type::partial);
        CHECK_NOTHROW(fw_set_bad_sys_mask.add(fw_var));
        CHECK(fw_set_bad_sys_mask.type() == xia::pixie::firmware::firmware_set::set_type::release);
        CHECK_THROWS_WITH_AS(xia::pixie::firmware::add(firmwares, fw_set_bad_sys_mask),
                             "firmware: set: add: released set already exists: module:17-500-14"
                             " release:2.2.2", fw_error);
        CHECK_NOTHROW(firmwares.clear());

        /* Duplicate known device */
        xia::pixie::firmware::firmware_set fw_set_par;
        CHECK_NOTHROW(fw_set_par.add(fw_sys));
        CHECK_THROWS_WITH_AS(
            fw_set_par.add(fw_sys),
            "firmware: add: duplicate firmware: fw:17-500-14 device:sys",
            fw_error);

        /* Firmware set with mismatching tag */
        xia::pixie::firmware::firmware_set fw_set_mixed_tags;
        CHECK_NOTHROW(fw_set_mixed_tags.add(fw_sys));
        CHECK_THROWS_WITH_AS(fw_set_mixed_tags.add(fw_sys_user),
                             "firmware: add: invalid tag: module:17-500-14 fw:15-250-12",
                             fw_error);

        /* Test Rev B/C/D firmware */
        auto fw_sys_d = xia::pixie::firmware::parse(fw_release, fw_def_rev_d[0], ',');
        CHECK(fw_sys_d.tag == "13-100-12");
        CHECK(fw_sys_d.device.name == "sys");
        CHECK(fw_sys_d.device.mod_revision == 13);
        CHECK(fw_sys_d.device.mod_adc_msps == 100);
        CHECK(fw_sys_d.device.mod_adc_bits == 12);
        CHECK(fw_sys_d.filename == "firmware/syspixie16_revdgeneral_adc100mhz_r30882.bin");

        auto fw_fippi_d = xia::pixie::firmware::parse(fw_release, fw_def_rev_d[1], ',');
        CHECK(fw_fippi_d.tag == "13-100-12");
        CHECK(fw_fippi_d.device.name == "fippi");
        CHECK(fw_fippi_d.device.mod_revision == 13);
        CHECK(fw_fippi_d.device.mod_adc_msps == 100);
        CHECK(fw_fippi_d.device.mod_adc_bits == 12);
        CHECK(fw_fippi_d.filename == "firmware/fippixie16_revdgeneral_12b100m_r30944.bin");

        auto fw_dsp_d = xia::pixie::firmware::parse(fw_release, fw_def_rev_d[2], ',');
        CHECK(fw_dsp_d.tag == "13-100-12");
        CHECK(fw_dsp_d.device.name == "dsp");
        CHECK(fw_dsp_d.device.mod_revision == 13);
        CHECK(fw_dsp_d.device.mod_adc_msps == 100);
        CHECK(fw_dsp_d.device.mod_adc_bits == 12);
        CHECK(fw_dsp_d.filename == "dsp/Pixie16DSP_revdgeneral_12b100m_r30918.ldr");

        auto fw_var_d = xia::pixie::firmware::parse(fw_release, fw_def_rev_d[3], ',');
        CHECK(fw_var_d.tag == "13-100-12");
        CHECK(fw_var_d.device.name == "var");
        CHECK(fw_var_d.device.mod_revision == 13);
        CHECK(fw_var_d.device.mod_adc_msps == 100);
        CHECK(fw_var_d.device.mod_adc_bits == 12);
        CHECK(fw_var_d.filename == "dsp/Pixie16DSP_revdgeneral_12b100m_r30918.var");

        xia::pixie::firmware::firmware_set fw_set_rev_d;
        CHECK(fw_set_rev_d.size() == 0);
        CHECK(fw_set_rev_d.device_count() == 0);
        CHECK_NOTHROW(fw_set_rev_d.add(fw_sys_d));
        CHECK(fw_set_rev_d.size() == 1);
        CHECK(fw_set_rev_d.device_count() == 1);
        CHECK_NOTHROW(fw_set_rev_d.add(fw_fippi_d));
        CHECK(fw_set_rev_d.size() == 2);
        CHECK(fw_set_rev_d.device_count() == 2);
        CHECK_NOTHROW(fw_set_rev_d.add(fw_dsp_d));
        CHECK(fw_set_rev_d.size() == 3);
        CHECK(fw_set_rev_d.device_count() == 3);
        CHECK_NOTHROW(fw_set_rev_d.add(fw_var_d));
        CHECK(fw_set_rev_d.size() == 4);
        CHECK(fw_set_rev_d.device_count() == 4);
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_rev_d));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["13-100-12"].size() == 1);
        CHECK_NOTHROW(fw_set_rev_d.clear());
        CHECK_NOTHROW(firmwares.clear());

        xia::pixie::firmware::firmware_set fw_set_sys_d;
        CHECK_NOTHROW(fw_set_sys_d.add(fw_sys_d));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_sys_d));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["13-100-12"].size() == 1);
        CHECK(firmwares["13-100-12"][0]->device_count() == 1);
        xia::pixie::firmware::firmware_set fw_set_fippi_d;
        CHECK_NOTHROW(fw_set_fippi_d.add(fw_fippi_d));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_fippi_d));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["13-100-12"].size() == 1);
        CHECK(firmwares["13-100-12"][0]->device_count() == 2);
        xia::pixie::firmware::firmware_set fw_set_dsp_d;
        CHECK_NOTHROW(fw_set_dsp_d.add(fw_dsp_d));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_dsp_d));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["13-100-12"].size() == 1);
        CHECK(firmwares["13-100-12"][0]->device_count() == 3);
        xia::pixie::firmware::firmware_set fw_set_var_d;
        CHECK_NOTHROW(fw_set_var_d.add(fw_var_d));
        CHECK_NOTHROW(xia::pixie::firmware::add(firmwares, fw_set_var_d));
        CHECK(firmwares.size() == 1);
        CHECK(firmwares["13-100-12"].size() == 1);
        CHECK(firmwares["13-100-12"][0]->device_count() == 4);
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
