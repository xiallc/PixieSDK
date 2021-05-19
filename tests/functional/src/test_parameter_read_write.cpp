/**----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------**/
/// @file test_pixie_channel.cpp
/// @brief Functional tests for reading and writing parameters.
/// @author S. V. Paulauskas
/// @date May 12, 2021
#include <iostream>
#include <sstream>

#include <doctest.h>
#include <pixie_channel.hpp>
#include <pixie_log.hpp>
#include <simulation.hpp>

static xia::pixie::sim::crate crate;

static const std::vector<std::string> module_defs = {
    "device-number=0,slot=2, revision=13, eeprom-format=1, serial-num=250, num-channels=16, adc-msps=100, adc-bits=16, adc-clk-div=1",
    "device-number=1,slot=3, revision=15, eeprom-format=1, serial-num=1000, num-channels=16, adc-msps=250, adc-bits=16, adc-clk-div=1",
    "device-number=2,slot=4, revision=15, eeprom-format=1, serial-num=1001, num-channels=16, adc-msps=500, adc-bits=14, adc-clk-div=1"
};

static const std::vector<std::string> firmware_defs = {
    "version=sim, revision=13, adc-msps=100, adc-bits=16, device=sys, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_sys.bin",
    "version=sim, revision=13, adc-msps=100, adc-bits=16, device=fippi, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_fippi.bin",
    "version=sim, revision=13, adc-msps=100, adc-bits=16, device=dsp, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.ldr",
    "version=sim, revision=13, adc-msps=100, adc-bits=16, device=var, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.var",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=sys, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_sys.bin",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=fippi, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_fippi.bin",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=dsp, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.ldr",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=var, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.var",
    "version=sim, revision=15, adc-msps=500, adc-bits=14, device=sys, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_sys.bin",
    "version=sim, revision=15, adc-msps=500, adc-bits=14, device=fippi, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_fippi.bin",
    "version=sim, revision=15, adc-msps=500, adc-bits=14, device=dsp, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.ldr",
    "version=sim, revision=15, adc-msps=500, adc-bits=14, device=var, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.var",
};

void setup_simulation() {
    xia::logging::start("log", "stdout", xia::log::level::debug, false);
    std::stringstream def;
    for (const auto& mod_def : module_defs)
        def << mod_def << std::endl;
    xia::pixie::sim::load_module_defs(def);
    for (const auto& item : firmware_defs) {
        auto fw = xia::pixie::firmware::parse(item, ',');
        xia::pixie::firmware::add(crate.firmware, fw);
    }
    crate.initialize(module_defs.size(), false);
    crate.set_firmware();
    crate.probe();
}

TEST_SUITE("xia::pixie::channel") {
    TEST_CASE("VOFFSET") {
        setup_simulation();
        const double expected_par = 0.77;
        const size_t expected_var = 49588;
        crate[1].write("VOFFSET", 0, expected_par);
        CHECK(crate[1].read_var("OffsetDAC", 0, 0) == expected_var);
        CHECK(doctest::Approx(crate[1].read("VOFFSET", 0)).epsilon(0.001) == expected_par);
    }
    TEST_CASE("XDT") {
        SUBCASE("100 MSPS - Too Small") {
            const double value = 0.0001;
            const double expected_par = 0.06;
            const double expected_var = 6;
            crate[0].write("XDT", 0, value);
            CHECK(crate[0].read_var("Xwait", 0, 0) == expected_var);
            CHECK(crate[0].read("XDT", 0) == expected_par);
        }
        SUBCASE("250 MSPS - Too Small") {
            const double value = 0.0001;
            const double expected_par = 0.08;
            const double expected_var = 8;
            crate[1].write("XDT", 0, value);
            CHECK(crate[1].read_var("Xwait", 0, 0) == expected_var);
            CHECK(crate[1].read("XDT", 0) == expected_par);
        }
        SUBCASE("250 MSPS") {
            const size_t value = 15;
            const size_t expected_var = 1504;
            const double expected_par = 15.04;
            crate[1].write("XDT", 0, value);
            CHECK(crate[1].read_var("Xwait", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("XDT", 0)) == expected_par);
        }
        SUBCASE("500 MSPS") {
            const size_t value = 14;
            const size_t expected_var = 1404;
            const double expected_par = 14.04;
            crate[2].write("XDT", 0, value);
            CHECK(crate[2].read_var("Xwait", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[2].read("XDT", 0)) == expected_par);
        }
    }
    TEST_CASE("TAU") {
        const double expected_par = 0.2;
        const size_t expected_var = 1045220556;
        crate[1].write("TAU", 0, expected_par);
        CHECK(crate[1].read_var("PreampTau", 0, 0) == expected_var);
        CHECK(doctest::Approx(crate[1].read("TAU", 0)).epsilon(0.001) == expected_par);
    }
}