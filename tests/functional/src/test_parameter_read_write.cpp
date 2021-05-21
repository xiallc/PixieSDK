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
/// @brief Functional tests for reading and writing parameters. ONLY testing non-trivial parameters.
/// @author S. V. Paulauskas
/// @date May 12, 2021
#include <cmath>
#include <sstream>

#include <doctest.h>
#include <pixie_channel.hpp>
#include <pixie_log.hpp>
#include <simulation.hpp>

static xia::pixie::sim::crate crate;

static const std::vector<std::string> module_defs = {
    "device-number=0,slot=2, revision=13, eeprom-format=1, serial-num=250, num-channels=16, adc-msps=100, adc-bits=16, adc-clk-div=1",
    "device-number=1,slot=3, revision=15, eeprom-format=1, serial-num=1000, num-channels=16, adc-msps=250, adc-bits=16, adc-clk-div=2",
    "device-number=2,slot=4, revision=15, eeprom-format=1, serial-num=1001, num-channels=16, adc-msps=500, adc-bits=14, adc-clk-div=5"};

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

TEST_SUITE("Parameter Reads and Writes") {
    TEST_CASE("Initialize Simulation") {
        ///@TODO We could maybe move this setup to a more communal spot, but this works for now.
        setup_simulation();
    }
    TEST_CASE("BASELINE_AVERAGE") {
        const double max_par = 16;
        const double max_var = 4294967280;
        SUBCASE("Happy path") {
            const double value = 3;
            const double expected_var = 4294967293;
            crate[1].write("BASELINE_AVERAGE", 0, value);
            CHECK(crate[1].read_var("Log2Bweight", 0, 0) == expected_var);
            CHECK(crate[1].read("BASELINE_AVERAGE", 0) == value);
        }
        SUBCASE("Negative") {
            crate[1].write("BASELINE_AVERAGE", 0, -3);
            CHECK(crate[1].read_var("Log2Bweight", 0, 0) == max_var);
            CHECK(crate[1].read("BASELINE_AVERAGE", 0) == max_par);
        }
        SUBCASE("Too Big") {
            crate[1].write("BASELINE_AVERAGE", 0, 17);
            CHECK(crate[1].read_var("Log2Bweight", 0, 0) == max_var);
            CHECK(crate[1].read("BASELINE_AVERAGE", 0) == max_par);
        }
    }
    TEST_CASE("BASELINE_PERCENT") {
        SUBCASE("Happy path") {
            const double expected = 10;
            crate[1].write("BASELINE_PERCENT", 0, expected);
            CHECK(crate[1].read_var("BaselinePercent", 0, 0) == expected);
            CHECK(doctest::Approx(crate[1].read("BASELINE_PERCENT", 0)).epsilon(0.001) == expected);
        }
        SUBCASE("Too Small") {
            const double value = 0.01;
            const double expected = 1;
            crate[1].write("BASELINE_PERCENT", 0, value);
            CHECK(crate[1].read_var("BaselinePercent", 0, 0) == expected);
            CHECK(doctest::Approx(crate[1].read("BASELINE_PERCENT", 0)).epsilon(0.001) == expected);
        }
        SUBCASE("Too Big") {
            const double value = 101;
            const double expected = 99;
            crate[1].write("BASELINE_PERCENT", 0, value);
            CHECK(crate[1].read_var("BaselinePercent", 0, 0) == expected);
            CHECK(doctest::Approx(crate[1].read("BASELINE_PERCENT", 0)).epsilon(0.001) == expected);
        }
    }
    TEST_CASE("BINFACTOR") {
        const double min_par  = 1;
        const double max_par = 6;
        const double min_var = 4294967295;
        const double max_var = 4294967290;
        SUBCASE("Happy path") {
            const double value = 3;
            const double expected_var = 4294967293;
            crate[1].write("BINFACTOR", 0, value);
            CHECK(crate[1].read_var("Log2Ebin", 0, 0) == expected_var);
            CHECK(crate[1].read("BINFACTOR", 0) == value);
        }
        SUBCASE("Negative") {
            crate[1].write("BINFACTOR", 0, -3);
            CHECK(crate[1].read_var("Log2Ebin", 0, 0) == max_var);
            CHECK(crate[1].read("BINFACTOR", 0) == max_par);
        }
        SUBCASE("Too Small") {
            crate[1].write("BINFACTOR", 0, 0);
            CHECK(crate[1].read_var("Log2Ebin", 0, 0) == min_var);
            CHECK(crate[1].read("BINFACTOR", 0) == min_par);
        }
        SUBCASE("Too Big") {
            crate[1].write("BINFACTOR", 0, 17);
            CHECK(crate[1].read_var("Log2Ebin", 0, 0) == max_var);
            CHECK(crate[1].read("BINFACTOR", 0) == max_par);
        }
    }
    TEST_CASE("CFDDelay") {
        const double min = 1;
        const double max = 63;
        const double fpga_clk_mhz = crate[1].configs[0].fpga_clk_mhz;
        SUBCASE("Happy Path") {
            const double value = 0.1;
            const double expected_var = std::round(value * fpga_clk_mhz);
            crate[1].write("CFDDelay", 0, value);
            CHECK(crate[1].read_var("CFDDelay", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("CFDDelay", 0)).epsilon(0.01) == value);
        }
        SUBCASE("Negative") {
            crate[1].write("CFDDelay", 0, -3);
            CHECK(crate[1].read_var("CFDDelay", 0, 0) == max);
            CHECK(crate[1].read("CFDDelay", 0) == max / fpga_clk_mhz);
        }
        SUBCASE("Too Small") {
            crate[1].write("CFDDelay", 0, 1.e-6);
            CHECK(crate[1].read_var("CFDDelay", 0, 0) == min);
            CHECK(crate[1].read("CFDDelay", 0) == min / fpga_clk_mhz);
        }
        SUBCASE("Too Big") {
            crate[1].write("CFDDelay", 0, 1.e6);
            CHECK(crate[1].read_var("CFDDelay", 0, 0) == max);
            CHECK(crate[1].read("CFDDelay", 0) == max / fpga_clk_mhz);
        }
    }
    TEST_CASE("CFDScale") {
        const double min = 0;
        const double max = 7;
        SUBCASE("Happy Path") {
            const double value = 3;
            crate[1].write("CFDScale", 0, value);
            CHECK(crate[1].read_var("CFDScale", 0, 0) == value);
            CHECK(crate[1].read("CFDScale", 0) == value);
        }
        SUBCASE("Negative") {
            crate[1].write("CFDScale", 0, -3);
            CHECK(crate[1].read_var("CFDScale", 0, 0) == max);
            CHECK(crate[1].read("CFDScale", 0) == max);
        }
        SUBCASE("Too Small") {
            crate[1].write("CFDScale", 0, 1.e-6);
            CHECK(crate[1].read_var("CFDScale", 0, 0) == min);
            CHECK(crate[1].read("CFDScale", 0) == min);
        }
        SUBCASE("Too Big") {
            crate[1].write("CFDScale", 0, 1.e6);
            CHECK(crate[1].read_var("CFDScale", 0, 0) == max);
            CHECK(crate[1].read("CFDScale", 0) == max);
        }
    }
    TEST_CASE("CFDThresh") {
        SUBCASE("Happy Path") {
            const double value = 10;
            crate[1].write("CFDThresh", 0, value);
            CHECK(crate[1].read_var("CFDThresh", 0, 0) == value);
            CHECK(crate[1].read("CFDThresh", 0) == value);
        }
        SUBCASE("Too Small") {
            CHECK_THROWS_AS(crate[1].write("CFDThresh", 0, 0.2), xia::pixie::error::error);
        }
        SUBCASE("Too Big") {
            CHECK_THROWS_AS(crate[1].write("CFDThresh", 0, 1.e6), xia::pixie::error::error);
        }
    }
    TEST_CASE("ChanTrigStretch") {
        SUBCASE("Happy Path - 250 MSPS") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("ChanTrigStretch", 0, value);
            CHECK(crate[1].read_var("ChanTrigStretch", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("ChanTrigStretch", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Too Small") {
            const double expected_var = 1;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("ChanTrigStretch", 0, 0.00000001);
            CHECK(crate[1].read_var("ChanTrigStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("ChanTrigStretch", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            const double expected_var = 4095;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("ChanTrigStretch", 0, 10000000);
            CHECK(crate[1].read_var("ChanTrigStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("ChanTrigStretch", 0) == expected_par);
        }
    }
    TEST_CASE("ENERGY_FLATTOP") {}
    TEST_CASE("ENERGY_RISETIME") {}
    TEST_CASE("energy_risetime_flattop") {}
    TEST_CASE("ExtTrigStretch") {
        SUBCASE("Happy Path - 250 MSPS") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("ExtTrigStretch", 0, value);
            CHECK(crate[1].read_var("ExtTrigStretch", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("ExtTrigStretch", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Too Small") {
            const double expected_var = 1;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("ExtTrigStretch", 0, 0.00000001);
            CHECK(crate[1].read_var("ExtTrigStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("ExtTrigStretch", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            const double expected_var = 4095;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("ExtTrigStretch", 0, 10000000);
            CHECK(crate[1].read_var("ExtTrigStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("ExtTrigStretch", 0) == expected_par);
        }
    }
    TEST_CASE("ExternDelayLen") {
        const double max_bcd = 255;
        const double max_fh = 511;
        SUBCASE("Rev D - 100 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[0].configs[0].fpga_clk_mhz);
            crate[0].write("ExternDelayLen", 0, value);
            CHECK(crate[0].read_var("ExternDelayLen", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[0].read("ExternDelayLen", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev D - 100 MSPS - Too Big") {
            crate[0].write("ExternDelayLen", 0, 10000000);
            CHECK(crate[0].read_var("ExternDelayLen", 0, 0) == max_bcd);
            CHECK(crate[0].read("ExternDelayLen", 0) == max_bcd / crate[0].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("ExternDelayLen", 0, value);
            CHECK(crate[1].read_var("ExternDelayLen", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("ExternDelayLen", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev F - 250 MSPS - Negative") {
            crate[1].write("ExternDelayLen", 0, -0.3);
            CHECK(crate[1].read_var("ExternDelayLen", 0, 0) == max_fh);
            CHECK(crate[1].read("ExternDelayLen", 0) == max_fh / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Too Big") {
            crate[1].write("ExternDelayLen", 0, 10000000);
            CHECK(crate[1].read_var("ExternDelayLen", 0, 0) == max_fh);
            CHECK(crate[1].read("ExternDelayLen", 0) == max_fh / crate[1].configs[0].fpga_clk_mhz);
        }
    }
    TEST_CASE("FASTTRIGBACKLEN") {
        const double min_100 = 1;
        const double min_250 = 2;
        const double max = 4095;
        SUBCASE("Rev D - 100 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[0].configs[0].fpga_clk_mhz);
            crate[0].write("FASTTRIGBACKLEN", 0, value);
            CHECK(crate[0].read_var("FastTrigBackLen", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[0].read("FASTTRIGBACKLEN", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev D - 100 MSPS - Too Small") {
            crate[0].write("FASTTRIGBACKLEN", 0, 1e-6);
            CHECK(crate[0].read_var("FastTrigBackLen", 0, 0) == min_100);
            CHECK(crate[0].read("FASTTRIGBACKLEN", 0) ==
                  min_100 / crate[0].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev D - 100 MSPS - Too Big") {
            crate[0].write("FASTTRIGBACKLEN", 0, 1e6);
            CHECK(crate[0].read_var("FastTrigBackLen", 0, 0) == max);
            CHECK(crate[0].read("FASTTRIGBACKLEN", 0) == max / crate[0].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("FASTTRIGBACKLEN", 0, value);
            CHECK(crate[1].read_var("FastTrigBackLen", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("FASTTRIGBACKLEN", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev F - 250 MSPS - Negative") {
            crate[1].write("FASTTRIGBACKLEN", 0, -0.3);
            CHECK(crate[1].read_var("FastTrigBackLen", 0, 0) == max);
            CHECK(crate[1].read("FASTTRIGBACKLEN", 0) == max / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Too Small") {
            crate[1].write("FASTTRIGBACKLEN", 0, 1e-6);
            CHECK(crate[1].read_var("FastTrigBackLen", 0, 0) == min_250);
            CHECK(crate[1].read("FASTTRIGBACKLEN", 0) ==
                  min_250 / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Too Big") {
            crate[1].write("FASTTRIGBACKLEN", 0, 1e6);
            CHECK(crate[1].read_var("FastTrigBackLen", 0, 0) == max);
            CHECK(crate[1].read("FASTTRIGBACKLEN", 0) == max / crate[1].configs[0].fpga_clk_mhz);
        }
    }
    TEST_CASE("FtrigoutDelay") {
        const double max_bcd = 255;
        const double max_fh = 511;
        SUBCASE("Rev D - 100 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[0].configs[0].fpga_clk_mhz);
            crate[0].write("FtrigoutDelay", 0, value);
            CHECK(crate[0].read_var("FtrigoutDelay", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[0].read("FtrigoutDelay", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev D - 100 MSPS - Too Big") {
            crate[0].write("FtrigoutDelay", 0, 10000000);
            CHECK(crate[0].read_var("FtrigoutDelay", 0, 0) == max_bcd);
            CHECK(crate[0].read("FtrigoutDelay", 0) == max_bcd / crate[0].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Happy Path") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("FtrigoutDelay", 0, value);
            CHECK(crate[1].read_var("FtrigoutDelay", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("FtrigoutDelay", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Rev F - 250 MSPS - Negative") {
            crate[1].write("FtrigoutDelay", 0, -0.3);
            CHECK(crate[1].read_var("FtrigoutDelay", 0, 0) == max_fh);
            CHECK(crate[1].read("FtrigoutDelay", 0) == max_fh / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Rev F - 250 MSPS - Too Big") {
            crate[1].write("FtrigoutDelay", 0, 10000000);
            CHECK(crate[1].read_var("FtrigoutDelay", 0, 0) == max_fh);
            CHECK(crate[1].read("FtrigoutDelay", 0) == max_fh / crate[1].configs[0].fpga_clk_mhz);
        }
    }
    TEST_CASE("INTEGRATOR") {
        ///@NOTE This variable is disabled at the time of writing the test.
        CHECK_THROWS_AS(crate[1].write("INTEGRATOR", 0, 0.3), xia::pixie::error::error);
    }
    TEST_CASE("QDCLen") {
        const double min = 1;
        const double max = 32767;
        const double value = 1.5;
        SUBCASE("Rev F - 500 MSPS - Happy Path") {
            double expected_var = value * (crate[2].configs[0].adc_msps / 5.);
            crate[2].write("QDCLen0", 0, value);
            CHECK(crate[2].read_var("QDCLen0", 0, 0) == expected_var);
            CHECK(crate[2].read("QDCLen0", 0) == value);
        }

        const double adc_msps = crate[1].configs[0].adc_msps;
        SUBCASE("Rev F - 250 MSPS - Happy Path") {
            const double expected_var = value * adc_msps;
            crate[1].write("QDCLen0", 0, value);
            CHECK(crate[1].read_var("QDCLen0", 0, 0) == expected_var);
            CHECK(crate[1].read("QDCLen0", 0) == value);
        }
        SUBCASE("Rev F - 250 MSPS - Negative") {
            crate[1].write("QDCLen0", 0, value * -1.);
            CHECK(crate[1].read_var("QDCLen0", 0, 0) == max);
            CHECK(crate[1].read("QDCLen0", 0) == max / adc_msps);
        }
        SUBCASE("Rev F - 250 MSPS - Too Small") {
            crate[1].write("QDCLen0", 0, 1.e-6);
            CHECK(crate[1].read_var("QDCLen0", 0, 0) == min);
            CHECK(crate[1].read("QDCLen0", 0) == min / adc_msps);
        }
        SUBCASE("Rev F - 250 MSPS - Too Big") {
            crate[1].write("QDCLen0", 0, 1.e6);
            CHECK(crate[1].read_var("QDCLen0", 0, 0) == max);
            CHECK(crate[1].read("QDCLen0", 0) == max / adc_msps);
        }
    }
    TEST_CASE("TAU") {
        const double expected_par = 0.2;
        const size_t expected_var = 1045220556;
        crate[1].write("TAU", 0, expected_par);
        CHECK(crate[1].read_var("PreampTau", 0, 0) == expected_var);
        CHECK(doctest::Approx(crate[1].read("TAU", 0)).epsilon(0.001) == expected_par);
    }
    TEST_CASE("TRACE_DELAY") {}
    TEST_CASE("TRACE_LENGTH") {}
    TEST_CASE("TRIGGER_FLATTOP") {}
    TEST_CASE("TRIGGER_RISETIME") {}
    TEST_CASE("TRIGGER_THRESHOLD") {}
    TEST_CASE("update_fifo") {}
    TEST_CASE("VetoStretch") {
        SUBCASE("Happy Path - 250 MSPS") {
            const double value = 0.3;
            double expected_var = std::round(value * crate[1].configs[0].fpga_clk_mhz);
            crate[1].write("VetoStretch", 0, value);
            CHECK(crate[1].read_var("VetoStretch", 0, 0) == expected_var);
            CHECK(doctest::Approx(crate[1].read("VetoStretch", 0)).epsilon(0.005) == value);
        }
        SUBCASE("Too Small") {
            const double expected_var = 1;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("VetoStretch", 0, 0.00000001);
            CHECK(crate[1].read_var("VetoStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("VetoStretch", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            const double expected_var = 4095;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("VetoStretch", 0, 10000000);
            CHECK(crate[1].read_var("VetoStretch", 0, 0) == expected_var);
            CHECK(crate[1].read("VetoStretch", 0) == expected_par);
        }
    }
    TEST_CASE("VOFFSET") {
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
}