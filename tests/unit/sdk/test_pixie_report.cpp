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

/** @file test_pixie_report.cpp
 * @brief Provides test coverage for report namespace.
 */

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/config.hpp>
#include <pixie/format.hpp>
#include <pixie/reports/reports.hpp>
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
    "device-number=2 slot=10 revision=17 eeprom-format=2 serial-num=2034 num-channels=32 "
    "adc-bits=14 adc-msps=250 adc-clk-div=2 db_pid=4 db_count=4",
    "device-number=3 slot=13 revision=12 eeprom-format=1 serial-num=1001 num-channels=16 "
    "adc-bits=12 adc-msps=100 adc-clk-div=1"};

static const size_t test_modules = module_def.size();

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
     "device=sys, mask=1, file=syspixie16_revfgeneral_adc500mhz_r33339.bin",
     "release=2.2.2, version=r42081, revision=17, adc-msps=250, adc-bits=14, " \
     "device=fippi, mask=0xf, file=fippixie16_revfgeneral_14b500m_r42081.bin",
     "release=2.2.2, version=r41847, revision=17, adc-msps=250, adc-bits=14, " \
     "device=dsp, mask=1, file=Pixie16DSP_revfgeneral_14b500m_r41847.ldr",
     "release=2.2.2, version=r41847, revision=17, adc-msps=250, adc-bits=14, " \
     "device=var, mask=1, file=Pixie16DSP_revfgeneral_14b500m_r41847.var"},
    {"release=0.9.1, version=r30882, revision=12, adc-msps=100, adc-bits=12, " \
     "device=sys, mask=1, file=syspixie16_revdgeneral_adc100mhz_r30882.bin",
     "release=0.9.1, version=r30944, revision=12, adc-msps=100, adc-bits=12, " \
     "device=fippi, mask=0xf, file=fippixie16_revdgeneral_12b100m_r30944.bin",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=dsp, mask=1, file=Pixie16DSP_revdgeneral_12b100m_r30918.ldr",
     "release=0.9.1, version=r30918, revision=12, adc-msps=100, adc-bits=12, " \
     "device=var, mask=1, file=Pixie16DSP_revdgeneral_12b100m_r30918.var"}
};

static const std::string crate_report =
  "[{\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":2,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1034\"},"
  "{\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":6,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1035\"},"
  "{\"adc_freq\":250,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":10,\"eeprom_version\":2,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":32,"
  "\"minor_revision\":0,\"model\":\"H (17)\",\"num_channels\":32,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"2034\"},"
  "{\"adc_freq\":100,\"adc_res\":12,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":13,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"C (12)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1001\"}]";

static const std::string module_report =
  "{\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":2,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1034\"}";

static const std::string crate_slot_report_1 =
  "[{\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":2,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1034\"}]";

static const std::string crate_slot_report_2 =
  "[{\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":2,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1034\"},{"
  "\"adc_freq\":500,\"adc_res\":14,\"board_revision\":-1,\"crate_revision\":-1,"
  "\"crate_slot\":6,\"eeprom_version\":1,\"fifo\":{\"bandwidth\":0,\"buffers\":100,"
  "\"dma_trig\":1024,\"hold\":10000,\"idle_wait\":150000,\"run_wait\":5000},"
  "\"i2c_read_period\":100.0,\"major_revision\":0,\"max_channels\":16,"
  "\"minor_revision\":0,\"model\":\"F (15)\",\"num_channels\":16,"
  "\"old_io_cpld_version\":false,\"online\":true,\"pci_bus\":255,\"pci_slot\":255,"
  "\"product\":\"Pixie-16\",\"running\":false,\"serial_number\":\"1035\"}]";

static void test_setup() {
    xia::logging::start("log", "test-module.txt", false);
    xia::logging::set_level(xia::log::level::info);
    for (auto& def : module_def) {
        xia::pixie::sim::add_module_def(def);
    }
}

TEST_SUITE("xia::report") {
    TEST_CASE("Setup") {
        test_setup();
    }
    TEST_CASE("Reports: Crate report") {
        using namespace xia::pixie;
        std::string rpt;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_NOTHROW(crate->boot());
        CHECK(crate->num_online == test_modules);
        CHECK(crate->num_offline == 0);
        xia::reports::report(crate, rpt);
        CHECK(rpt == crate_report);
    }
    TEST_CASE("Reports: Module report") {
        using namespace xia::pixie;
        std::string rpt;
        sim::crate sim_crate;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_NOTHROW(crate->boot());
        CHECK(crate->num_online == test_modules);
        CHECK(crate->num_offline == 0);
        xia::reports::report(crate[0], rpt);
        CHECK(rpt == module_report);
    }
    TEST_CASE("Reports: Crate slots report") {
        using namespace xia::pixie;
        std::string rpt;
        sim::crate sim_crate;
        hw::slot_numbers slots;;
        sim::load_firmware_sets(sim_crate.firmware, firmware_defs);
        crate::view::module crate(sim_crate);
        CHECK_NOTHROW(crate->initialize());
        CHECK_NOTHROW(crate->probe());
        CHECK_NOTHROW(crate->boot());
        CHECK(crate->num_online == test_modules);
        CHECK(crate->num_offline == 0);
        xia::reports::report(crate, rpt, slots);
        CHECK(rpt == crate_report);
        slots.push_back(2);
        xia::reports::report(crate, rpt, slots);
        CHECK(rpt == crate_slot_report_1);
        slots.push_back(6);
        xia::reports::report(crate, rpt, slots);
        CHECK(rpt == crate_slot_report_2);
        slots.push_back(33);
        CHECK_THROWS_WITH_AS(xia::reports::report(crate, rpt, slots),
            "provided slot is out of range", crate_error);
    }
}
