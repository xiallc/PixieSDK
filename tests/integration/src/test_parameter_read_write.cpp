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
#include <pixie_error.hpp>
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

static const size_t max_fifo_length = 16380;
static const double a_big_value = 1e6;
static const double a_small_value = 1.e-6;

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
    for (auto& mod : crate.modules) {
        mod->write_var("FastFilterRange", xia::pixie::param::value_type(0), 0);
        mod->write_var("SlowFilterRange", xia::pixie::param::value_type(3), 0);
        //@TODO This is here temporarily to facilitate tests until P16-263 is done.
        mod->write_var("FIFOLength", xia::pixie::param::value_type(16380), 0);
    }
}

TEST_SUITE("Simulation Initialization") {
    TEST_CASE("Initialize Simulation") {
        ///@TODO We could maybe move this setup to a more communal spot, but this works for now.
        setup_simulation();
    }
}

TEST_SUITE("Channel Parameter Reads and Writes") {
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
        const double min_par = 1;
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
    TEST_CASE("TRACE_LENGTH") {
        SUBCASE("500 - Happy Path") {
            double value = 0.253;
            size_t expected_var = uint(value * crate[2].configs[0].adc_msps) / 10 * 10;
            double expected_par = double(expected_var) / crate[2].configs[0].adc_msps;
            crate[2].write("TRACE_LENGTH", 0, value);
            CHECK(crate[2].read_var("TraceLength", 0, 0) == expected_var);
            CHECK(crate[2].read("TRACE_LENGTH", 0) == expected_par);
        }
        SUBCASE("250 - Happy Path") {
            double value = 0.151;
            size_t expected_var = uint(value * crate[1].configs[0].adc_msps) / 2 * 2;
            double expected_par = double(expected_var) / crate[1].configs[0].adc_msps;
            crate[1].write("TRACE_LENGTH", 0, value);
            CHECK(crate[1].read_var("TraceLength", 0, 0) == expected_var);
            CHECK(crate[1].read("TRACE_LENGTH", 0) == expected_par);
        }
        SUBCASE("250 - Too Big") {
            double value = 1e6;
            size_t expected_var = max_fifo_length;
            double expected_par = double(expected_var) / crate[1].configs[0].adc_msps;
            crate[1].write("TRACE_LENGTH", 0, value);
            CHECK(crate[1].read_var("TraceLength", 0, 0) == max_fifo_length);
            CHECK(crate[1].read("TRACE_LENGTH", 0) == expected_par);
        }
    }
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

    ///@NOTE The remaining tests have overlapping parameter requirements
    TEST_CASE("TRIGGER_RISETIME") {
        crate[1].write_var("FastGap", 0, 0);

        const double max = 127;
        const double min = 2;
        SUBCASE("Happy Path") {
            const double value = 0.159;
            const size_t expected_var = 20;
            const double expected_par = 0.16;
            crate[1].write("TRIGGER_RISETIME", 0, value);
            CHECK(crate[1].read_var("FastLength", 0, 0) == expected_var);
            CHECK(crate[1].read("TRIGGER_RISETIME", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            crate[1].write("TRIGGER_RISETIME", 0, a_big_value);
            CHECK(crate[1].read_var("FastLength", 0, 0) == max);
            CHECK(crate[1].read("TRIGGER_RISETIME", 0) == max / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Too Small") {
            crate[1].write("TRIGGER_RISETIME", 0, a_small_value);
            CHECK(crate[1].read_var("FastLength", 0, 0) == min);
            CHECK(crate[1].read("TRIGGER_RISETIME", 0) == min / crate[1].configs[0].fpga_clk_mhz);
        }
        SUBCASE("Too Small w/ Gap out of range") {
            crate[1].write_var("FastGap", 126, 0);
            crate[1].write("TRIGGER_RISETIME", 0, a_small_value);
            CHECK(crate[1].read_var("FastLength", 0, 0) == min);
            CHECK(crate[1].read_var("FastGap", 0, 0) == max - min);
            CHECK(crate[1].read("TRIGGER_RISETIME", 0) == min / crate[1].configs[0].fpga_clk_mhz);
        }
    }
    TEST_CASE("TRIGGER_FLATTOP") {
        const double max = 127;
        SUBCASE("Happy Path") {
            const double value = 0.159;
            const size_t expected_var = 20;
            const double expected_par = 0.16;
            crate[1].write("TRIGGER_FLATTOP", 0, value);
            CHECK(crate[1].read_var("FastGap", 0, 0) == expected_var);
            CHECK(crate[1].read("TRIGGER_FLATTOP", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            const double fast_length = 12;
            const double expected_var = max - fast_length;
            const double expected_par = expected_var / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write_var("FastLength", fast_length, 0);
            crate[1].write("TRIGGER_FLATTOP", 0, a_big_value);
            CHECK(crate[1].read_var("FastGap", 0, 0) == expected_var);
            CHECK(crate[1].read("TRIGGER_FLATTOP", 0) == expected_par);
        }
    }
    TEST_CASE("TRIGGER_THRESHOLD") {
        const double fast_length_var = 12;
        const double max = 65535;
        crate[1].write_var("FastLength", fast_length_var, 0);
        SUBCASE("Happy Path") {
            const double value = 0.5;
            const size_t expected_var = value * fast_length_var * crate[1].configs[0].adc_clk_div;
            const double expected_par =
                expected_var / (fast_length_var * crate[1].configs[0].adc_clk_div);
            crate[1].write("TRIGGER_THRESHOLD", 0, value);
            CHECK(crate[1].read_var("FastThresh", 0, 0) == expected_var);
            CHECK(crate[1].read("TRIGGER_THRESHOLD", 0) == expected_par);
        }
        SUBCASE("Too Big") {
            const size_t expected_var = max / (max - 0.5) * max;
            const double expected_par =
                expected_var / (fast_length_var * crate[1].configs[0].adc_clk_div);
            crate[1].write("TRIGGER_THRESHOLD", 0, a_big_value);
            CHECK(crate[1].read_var("FastThresh", 0, 0) == expected_var);
            CHECK(crate[1].read("TRIGGER_THRESHOLD", 0) == expected_par);
        }
    }
    TEST_CASE("update_fifo") {
        const size_t peak_sep = 12;
        crate[1].write_var("PeakSep", peak_sep, 0);

        const size_t trace_delay = 10;
        const size_t sfr_mask = 1 << crate[1].read_var("SlowFilterRange", 0, false);
        const size_t ffr_mask = 1 << crate[1].read_var("FastFilterRange", 0, false);
        const double trigger_delay = (peak_sep - 1) * sfr_mask;
        const double paf_length = trigger_delay / ffr_mask + trace_delay;

        SUBCASE("Happy Path") {
            crate[1].channels[0].update_fifo(trace_delay);
            CHECK(crate[1].read_var("PAFlength", 0, 0) == paf_length);
            CHECK(crate[1].read_var("TriggerDelay", 0, 0) == trigger_delay);
        }
        SUBCASE("paf_length too big") {
            crate[1].channels[0].update_fifo(a_big_value);
            CHECK(crate[1].read_var("PAFlength", 0, 0) == max_fifo_length - 1);
            CHECK(crate[1].read_var("TriggerDelay", 0, 0) ==
                  uint32_t(max_fifo_length) - 1 - uint32_t(a_big_value));
        }
    }
    TEST_CASE("TRACE_DELAY") {
        const double max = 1023;
        const size_t peak_sep = 12;
        crate[1].write_var("PeakSep", peak_sep, 0);

        SUBCASE("Happy Path") {
            crate[1].write("TRACE_DELAY", 0, 0.051);
            CHECK(crate[1].read("TRACE_DELAY", 0) == 0.048);
        }
        SUBCASE("Too Big - Bigger than Trace Length") {
            double trace_length = 1000;
            crate[1].write_var("TraceLength", size_t(trace_length), 0);
            double expected_par = trace_length * 0.5 / crate[1].configs[0].fpga_clk_mhz;
            crate[1].write("TRACE_DELAY", 0, a_big_value);
            CHECK(crate[1].read("TRACE_DELAY", 0) == expected_par);
        }
        SUBCASE("Too Big - Smaller than Trace Length") {
            crate[1].write_var("TraceLength", size_t(max * 1.5), 0);
            crate[1].write("TRACE_DELAY", 0, 8.2);
            CHECK(crate[1].read("TRACE_DELAY", 0) == max / crate[1].configs[0].fpga_clk_mhz);
        }
    }
    TEST_CASE("ENERGY_RISETIME") {
        const double max = 127;
        const double min = 2;
        const uint32_t slow_gap = 3;
        crate[1].write_var("SlowGap", slow_gap, 0);

        SUBCASE("Happy Path") {
            crate[1].write("ENERGY_RISETIME", 0, 0.575);
            CHECK(crate[1].read_var("SlowLength", 0, 0) == 9);
            CHECK(crate[1].read("ENERGY_RISETIME", 0) == 0.576);
        }
        SUBCASE("Too Big") {
            crate[1].write("ENERGY_RISETIME", 0, a_big_value);
            CHECK(crate[1].read_var("SlowLength", 0, 0) == max - slow_gap);
            CHECK(crate[1].read("ENERGY_RISETIME", 0) == 7.936);
        }
        SUBCASE("Too Small") {
            crate[1].write("ENERGY_RISETIME", 0, a_small_value);
            CHECK(crate[1].read_var("SlowLength", 0, 0) == min);
            CHECK(crate[1].read("ENERGY_RISETIME", 0) == 0.128);
        }
        SUBCASE("Too Small w/ Gap out of range") {
            crate[1].write_var("SlowGap", 126, 0);
            crate[1].write("ENERGY_RISETIME", 0, a_small_value);
            CHECK(crate[1].read_var("SlowLength", 0, 0) == min);
            CHECK(crate[1].read_var("SlowGap", 0, 0) == max - min);
            CHECK(crate[1].read("ENERGY_RISETIME", 0) == 0.128);
        }
    }
    TEST_CASE("ENERGY_FLATTOP") {
        const double max = 127;
        const double min = 3;
        const uint32_t slow_length = 12;
        crate[1].write_var("SlowLength", slow_length, 0);

        SUBCASE("Happy Path") {
            crate[1].write("ENERGY_FLATTOP", 0, 0.5);
            CHECK(crate[1].read_var("SlowGap", 0, 0) == 8);
            CHECK(crate[1].read("ENERGY_FLATTOP", 0) == 0.512);
        }
        SUBCASE("Too Big") {
            crate[1].write("ENERGY_FLATTOP", 0, a_big_value);
            CHECK(crate[1].read_var("SlowGap", 0, 0) == max - slow_length);
            CHECK(crate[1].read("ENERGY_FLATTOP", 0) == 7.36);
        }
        SUBCASE("Too Small") {
            crate[1].write("ENERGY_FLATTOP", 0, a_small_value);
            CHECK(crate[1].read_var("SlowGap", 0, 0) == min);
            CHECK(crate[1].read("ENERGY_FLATTOP", 0) == 0.192);
        }
        SUBCASE("Too Small w/ Gap out of range") {
            crate[1].write_var("SlowLength", 126, 0);
            crate[1].write("ENERGY_FLATTOP", 0, a_small_value);
            CHECK(crate[1].read_var("SlowGap", 0, 0) == min);
            CHECK(crate[1].read_var("SlowLength", 0, 0) == max - min);
            CHECK(crate[1].read("ENERGY_FLATTOP", 0) == 0.192);
        }
    }
    TEST_CASE("energy_risetime_flattop") {
        crate[1].write_var("SlowLength", 12, 0);

        SUBCASE("Bad Parameter") {
            CHECK_THROWS_AS(crate[1].channels[0].energy_risetime_flattop(
                                xia::pixie::param::channel_param::voffset, 1.2),
                            xia::pixie::error::error);
        }
        SUBCASE("SFR = 1") {
            crate[1].write_var("SlowFilterRange", 1, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 12);
        }
        SUBCASE("SFR = 2") {
            crate[1].write_var("SlowFilterRange", 2, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 13);
        }
        SUBCASE("SFR = 3") {
            crate[1].write_var("SlowFilterRange", 3, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 13);
        }
        SUBCASE("SFR = 4") {
            crate[1].write_var("SlowFilterRange", 4, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 14);
        }
        SUBCASE("SFR = 5") {
            crate[1].write_var("SlowFilterRange", 5, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 15);
        }
        SUBCASE("SFR = 6") {
            crate[1].write_var("SlowFilterRange", 6, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 14);
        }
        SUBCASE("SFR = 7") {
            crate[1].write_var("SlowFilterRange", 7, 0);
            crate[1].channels[0].energy_risetime_flattop(
                xia::pixie::param::channel_param::energy_flattop, a_small_value);
            CHECK(crate[1].read_var("PeakSep", 0, 0) == 15);
            CHECK(crate[1].read_var("PeakSample", 0, 0) == 13);
        }
    }
}
