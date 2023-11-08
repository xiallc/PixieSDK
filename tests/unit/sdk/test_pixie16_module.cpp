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

/** @file test_pixie16_module.cpp
 * @brief
 */

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/crate-view.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/sim.hpp>

using crate_error = xia::pixie::crate::error;

static const std::vector<std::string> module_def = {
    "device-number=0 slot=2 revision=15 eeprom-format=1 serial-num=1034 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=1 slot=6 revision=15 eeprom-format=1 serial-num=1035 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=2 slot=10 revision=17 eeprom-format=1 serial-num=2034 num-channels=32 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=3 slot=13 revision=12 eeprom-format=1 serial-num=1001 num-channels=16 "
    "adc-bits=12 adc-msps=100 adc-clk-div=1"};

static const size_t test_modules = module_def.size();

static const xia::pixie::sim::firmware_set_defs firmware_defs = {
    {"version=r33330, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "version=r42070, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "version=r41807, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "version=r41840, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.1.1, version=r33339, revision=15, adc-msps=250, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=250, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=250, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.1.1, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.1, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.1, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=1.2.3, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=1.2.3, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=1.2.3, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.var"},
    {"release=1.1.4, version=r33339, revision=15, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
     "release=1.1.4, version=r42081, revision=15, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_12b250m_r42081.bin",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.ldr",
     "release=1.1.4, version=r41847, revision=15, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_12b250m_r41847.var"},
    {"release=2.2.2, version=r133339, revision=17, adc-msps=500, adc-bits=14, " \
     "device=sys, mask=1, file=firmware/syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=2.2.2, version=r42081, revision=17, adc-msps=500, adc-bits=14, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=14, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=2.2.2, version=r41847, revision=17, adc-msps=500, adc-bits=14, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revfgeneral_14b500m_r41847.var"},
    {"release=0.9.1, version=r30882, revision=12, adc-msps=100, adc-bits=12, " \
     "device=sys, mask=1, file=firmware/syspixie16_revdgeneral_adc100mhz_r30882.bin",
     "release=0.9.1, version=r30944, revision=12, adc-msps=100, adc-bits=12, " \
     "device=fippi, mask=0xf, file=firmware/fippixie16_revdgeneral_12b100m_r30944.bin",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=dsp, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.ldr",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=var, mask=1, file=dsp/Pixie16DSP_revdgeneral_12b100m_r30918.var"}
};

static void test_setup() {
    xia::logging::start("log", "test-module.txt", false);
    xia::logging::set_level(xia::log::level::info);
    for (auto& def : module_def) {
        xia::pixie::sim::add_module_def(def);
    }
}

TEST_SUITE("Crate: modules") {
    TEST_CASE("SETUP") {
        test_setup();
    }
    TEST_CASE("empty") {
        using namespace xia::pixie;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK(crate.num_modules == 0);
        CHECK(crate.modules.size() == 0);
        CHECK(crate->revision == -1);
        CHECK(crate->busy() == false);
        CHECK(crate->users() == 0);
        CHECK_THROWS_WITH_AS(crate->probe(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate->ready(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate->probe(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate->find(crate->num_slots), "module slot not found", crate_error);
        CHECK_THROWS_WITH_AS(crate[0], "module number out of range: 0", crate_error);
        CHECK_THROWS_WITH_AS(crate.find(0), "module slot not found", crate_error);
    }
    TEST_CASE("initialize offline, boot") {
        using namespace xia::pixie;
        sim::crate sim_crate(false);
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK(crate.num_modules == test_modules);
        CHECK(crate->num_offline == test_modules);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate.num_modules == test_modules);
        CHECK(crate->num_offline == test_modules);
        CHECK_NOTHROW(crate->boot());
        CHECK(crate->num_online == test_modules);
        CHECK(crate->num_offline == 0);
        firmware::release_type release;
        firmware::firmware_set::set_type type;
        crate[0].firmware_release(release, type);
        CHECK(release.to_string() == "1.2.3");
        CHECK(type == firmware::firmware_set::set_type::release);
        module::module::boot_params params;
        params.boot_comms = false;
        params.boot_fippi = false;
        params.boot_dsp = true;
        firmware::firmware_set fw_set_1(release, "n/a");
        xia::pixie::firmware::find_filter filter(crate[0].get_fw_tag(), crate[0].slot);
        CHECK_NOTHROW(firmware::find(fw_set_1, crate->firmware, filter));
        CHECK(fw_set_1.valid() == true);
        CHECK_NOTHROW(crate[0].boot(params, fw_set_1));
        firmware::firmware_set fw_set_2;
        filter.set_release("1.1.4");
        CHECK_NOTHROW(firmware::find(fw_set_2, crate->firmware, filter));
        CHECK(fw_set_2.valid() == true);
        CHECK(fw_set_1.release != fw_set_2.release);
        CHECK_THROWS_WITH_AS(
            crate[0].boot(params, fw_set_2),
            "module: num=0,slot=2: partial boot: firmware does not match resident firmware", crate_error);
    }
    TEST_CASE("initialize") {
        using namespace xia::pixie;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_NOTHROW(crate->boot());
        CHECK(crate->num_online == test_modules);
        CHECK(crate->num_offline == 0);
        SUBCASE("Finding and offline") {
            CHECK_NOTHROW(crate.find(2));
            CHECK_THROWS_WITH_AS(crate[-1], "module number out of range: -1", crate_error);
            CHECK_THROWS_WITH_AS(crate[20], "module number out of range: 20", crate_error);
            CHECK_THROWS_WITH_AS(crate.find(7), "module slot not found", crate_error);
            CHECK(crate.num_modules == test_modules);
            CHECK(crate.modules.size() == test_modules);
            CHECK(crate->num_online == test_modules);
            CHECK(crate->num_offline == 0);
            CHECK(crate->backplane.sync_waiters.size() == crate->num_slots);
            CHECK_THROWS_WITH_AS(crate->set_offline(7), "module not seen as online",
                                 crate_error);
            CHECK_THROWS_WITH_AS(crate->set_offline(crate[7]), "module number out of range: 7",
                                 crate_error);
            CHECK_NOTHROW(crate->set_offline(crate[1]));
            CHECK(crate->backplane.sync_waiters.size() == crate->num_slots);
            CHECK_NOTHROW(crate->backplane_reinit());
            CHECK(crate.num_modules == test_modules);
            CHECK(crate.modules.size() == test_modules);
            CHECK(crate->num_online == test_modules - 1);
            CHECK(crate->num_offline == 1);
            CHECK(crate->backplane.sync_waiters.size() == crate->num_slots);
        }
        using namespace xia::pixie;
        SUBCASE("FIFO defaults") {
            CHECK(crate[0].fifo_buffers == module::module::default_fifo_buffers);
            CHECK(crate[0].fifo_run_wait_usecs == module::module::default_fifo_run_wait_usec);
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::default_fifo_idle_wait_usec);
            CHECK(crate[0].fifo_hold_usecs == module::module::default_fifo_hold_usec);
            CHECK(crate[0].fifo_dma_trigger_level ==
                  module::module::default_fifo_dma_trigger_level);
            CHECK(crate[0].fifo_bandwidth == 0);
        }
        SUBCASE("FIFO bandwidth") {
            CHECK_NOTHROW(crate[0].set_fifo_buffers(50));
            CHECK(crate[0].fifo_buffers == 50);
        }
        SUBCASE("FIFO run wait") {
            CHECK_NOTHROW(crate[0].set_fifo_run_wait(module::module::min_fifo_run_wait_usec));
            CHECK(crate[0].fifo_run_wait_usecs == module::module::min_fifo_run_wait_usec);
            CHECK_NOTHROW(crate[0].set_fifo_run_wait(module::module::max_fifo_run_wait_usec));
            CHECK(crate[0].fifo_run_wait_usecs == module::module::max_fifo_run_wait_usec);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_run_wait(module::module::min_fifo_run_wait_usec - 1),
                "module: num=0,slot=2: fifo: run wait value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_run_wait(module::module::max_fifo_run_wait_usec + 1),
                "module: num=0,slot=2: fifo: run wait value out of range", crate_error);
        }
        SUBCASE("FIFO idle wait") {
            CHECK_NOTHROW(crate[0].set_fifo_idle_wait(module::module::min_fifo_idle_wait_usec));
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::min_fifo_idle_wait_usec);
            CHECK_NOTHROW(crate[0].set_fifo_idle_wait(module::module::max_fifo_idle_wait_usec));
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::max_fifo_idle_wait_usec);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_idle_wait(module::module::min_fifo_idle_wait_usec - 1),
                "module: num=0,slot=2: fifo: idle wait value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_idle_wait(module::module::max_fifo_idle_wait_usec + 1),
                "module: num=0,slot=2: fifo: idle wait value out of range", crate_error);
        }
        SUBCASE("FIFO hold") {
            CHECK_NOTHROW(crate[0].set_fifo_hold(module::module::min_fifo_hold_usec));
            CHECK(crate[0].fifo_hold_usecs == module::module::min_fifo_hold_usec);
            CHECK_NOTHROW(crate[0].set_fifo_hold(module::module::max_fifo_hold_usec));
            CHECK(crate[0].fifo_hold_usecs == module::module::max_fifo_hold_usec);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_hold(module::module::min_fifo_hold_usec - 1),
                                 "module: num=0,slot=2: fifo: hold value out of range",
                                 crate_error);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_hold(module::module::max_fifo_hold_usec + 1),
                                 "module: num=0,slot=2: fifo: hold value out of range",
                                 crate_error);
        }
        SUBCASE("FIFO DMA trigger level") {
            CHECK_NOTHROW(
                crate[0].set_fifo_dma_trigger_level(module::module::min_fifo_dma_trigger_level));
            CHECK(crate[0].fifo_dma_trigger_level == module::module::min_fifo_dma_trigger_level);
            CHECK_NOTHROW(
                crate[0].set_fifo_dma_trigger_level(module::module::max_fifo_dma_trigger_level));
            CHECK(crate[0].fifo_dma_trigger_level == module::module::max_fifo_dma_trigger_level);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_dma_trigger_level(module::module::min_fifo_dma_trigger_level - 1),
                "module: num=0,slot=2: fifo: dma trigger level value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_dma_trigger_level(module::module::max_fifo_dma_trigger_level + 1),
                "module: num=0,slot=2: fifo: dma trigger level value out of range", crate_error);
        }
        SUBCASE("FIFO bandwidth") {
            CHECK_NOTHROW(crate[0].set_fifo_bandwidth(0));
            CHECK(crate[0].fifo_bandwidth == 0);
            CHECK_NOTHROW(crate[0].set_fifo_bandwidth(100));
            CHECK(crate[0].fifo_bandwidth == 100);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_bandwidth(101),
                                 "module: num=0,slot=2: fifo: bandwidth value out of range",
                                 crate_error);
        }
    }
    TEST_CASE("open and close") {
        using namespace xia::pixie;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_THROWS_WITH_AS(crate[0].open(0),
                             "module: num=0,slot=2: module has a vaddr", crate_error);
        CHECK_NOTHROW(crate[0].close());
        WARN_NOTHROW(crate[0].close());
        CHECK_NOTHROW(crate[1].close());
        CHECK_NOTHROW(crate[2].close());
        CHECK_NOTHROW(crate[3].close());
        CHECK_NOTHROW(crate[0].open(0));
        CHECK_NOTHROW(crate[1].open(1));
        CHECK_NOTHROW(crate[2].open(2));
        CHECK_NOTHROW(crate[3].open(3));
    }
    TEST_CASE("assign slots") {
        using namespace xia::pixie;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_THROWS_WITH_AS(crate.assign(module::number_slots{{0, 2}, {6, 6}}),
                             "module asignment numbers are not continuous", crate_error);
        CHECK_THROWS_WITH_AS(crate.assign(module::number_slots{{0, 2}, {1, 50}}),
                             "module asignment has invalid slot numbers", crate_error);
        CHECK_NOTHROW(crate.assign(module::number_slots{{0, 2}, {1, 6}, {2, 10}, {3, 13}}));
        CHECK(crate[0].slot == 2);
        CHECK(crate[1].slot == 6);
        CHECK(crate[2].slot == 10);
        CHECK(crate[3].slot == 13);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate[0].slot == 2);
        CHECK(crate[1].slot == 6);
        CHECK(crate[2].slot == 10);
        CHECK(crate[3].slot == 13);
        CHECK_NOTHROW(crate.assign(module::number_slots{{3, 2}, {2, 6}, {1, 10}, {0, 13}}));
        CHECK(crate[0].slot == 13);
        CHECK(crate[1].slot == 10);
        CHECK(crate[2].slot == 6);
        CHECK(crate[3].slot == 2);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate[0].slot == 13);
        CHECK(crate[1].slot == 10);
        CHECK(crate[2].slot == 6);
        CHECK(crate[3].slot == 2);
        CHECK_NOTHROW(crate.assign(module::number_slots{{1, 2}, {0, 6}, {2, 10}}));
        CHECK(crate.num_modules == test_modules - 1);
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK(crate[2].slot == 10);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK(crate[2].slot == 10);
        CHECK_NOTHROW(crate.assign(module::number_slots{{1, 2}, {0, 6}}));
        CHECK(crate.num_modules == test_modules - 2);
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK_NOTHROW(crate.assign(module::number_slots{{0, 6}}));
        CHECK(crate.num_modules == test_modules - 3);
        CHECK(crate[0].slot == 6);
        CHECK_NOTHROW(crate.assign(module::number_slots{{1, 2}, {0, 6}, {3, 10}, {2, 13}}));
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK(crate[2].slot == 13);
        CHECK(crate[3].slot == 10);
        CHECK_NOTHROW(crate->probe());
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK(crate[2].slot == 13);
        CHECK(crate[3].slot == 10);
        CHECK(crate.num_modules == test_modules);
    }
    TEST_CASE("run") {
        using namespace xia::pixie;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[1].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[2].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[3].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[0].run_end());
        CHECK_NOTHROW(crate[2].run_end());
        CHECK_NOTHROW(crate[1].run_end());
        CHECK_NOTHROW(crate[3].run_end());
    }
    TEST_CASE("backplane") {
        using namespace xia::pixie;
        using namespace xia::pixie::param;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        SUBCASE("wired-or trigger pullup") {
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP),
                "module: num=2,slot=10: wired-or trigger pullups leader role already taken: slot: 2",
                crate_error);
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP));
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("director mode") {
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD),
                "module: num=2,slot=10: director leader role already taken: slot: 2", crate_error);
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD));
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("run leader") {
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER),
                "module: num=2,slot=10: run leader role already taken: slot: 2", crate_error);
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("no synch wait") {
            CHECK(crate->backplane.sync_waits == 0);
            CHECK(crate->backplane.sync_waiters[crate[0].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[1].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[2].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[3].slot] == false);
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 0));
            CHECK(crate->backplane.sync_waits == 0);
            CHECK(crate->backplane.sync_waiters[crate[0].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[1].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[2].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[3].slot] == false);
            CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
            CHECK_NOTHROW(crate[0].run_end());
        }
        SUBCASE("incomplete synch wait") {
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 1));
            CHECK(crate->backplane.sync_waits == 1);
            CHECK(crate->backplane.sync_waiters[crate[0].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[1].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[2].slot] == false);
            CHECK(crate->backplane.sync_waiters[crate[3].slot] == false);
            CHECK_THROWS_WITH_AS(
                crate[0].start_histograms(hw::run::run_mode::new_run),
                "sync wait mode enabled and not all slots in the sync wait state: 1 of 4 waiting",
                crate_error);
            CHECK_NOTHROW(crate[0].run_end());
        };
        SUBCASE("complete synch wait with no leader") {
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[1].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[2].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[3].write(module_param::synch_wait, 1));
            CHECK(crate->backplane.sync_waits == test_modules);
            CHECK(crate->backplane.sync_waiters[crate[0].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[1].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[2].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[3].slot] == true);
            CHECK_THROWS_WITH_AS(
                crate[0].start_histograms(hw::run::run_mode::new_run),
                "sync wait mode enabled but no run leader slot is assigned",
                crate_error);
            CHECK_NOTHROW(crate[0].run_end());
        }
        SUBCASE("complete synch wait") {
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[1].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[2].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[3].write(module_param::synch_wait, 1));
            CHECK(crate->backplane.sync_waits == test_modules);
            CHECK(crate->backplane.sync_waiters[crate[0].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[1].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[2].slot] == true);
            CHECK(crate->backplane.sync_waiters[crate[3].slot] == true);
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
            CHECK_NOTHROW(crate[0].run_end());
        }
    }
    TEST_CASE("TEARDOWN") {
        xia::logging::stop("log");
    }
}
