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

static const std::vector<std::string> firmware_defs = {
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=sys, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_sys.bin",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=fippi, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_fippi.bin",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=dsp, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.ldr",
    "version=sim, revision=15, adc-msps=250, adc-bits=16, device=var, file=@PROJECT_SOURCE_DIR@/tests/assets/simulation/sim_dsp.var"};

void init_logging() {
    xia::logging::start("log", "stdout", xia::log::level::debug, false);
}

void setup_simulation(xia::pixie::sim::crate& crate, const size_t& revision, const size_t& num_channels, const size_t& msps,
                      const size_t& bits) {
    init_logging();
    std::stringstream def;
    def << "device-number=0, slot=2, revision=" << revision
        << ", eeprom-format=1, serial-num=1000, num-channels=" << num_channels
        << ", adc-msps=" << msps << ", adc-bits=" << bits << ", adc-clk-div=1";
    xia::pixie::sim::load_module_defs(def);
    for (const auto& item : firmware_defs) {
        auto fw = xia::pixie::firmware::parse(item, ',');
        xia::pixie::firmware::add(crate.firmware, fw);
    }
    crate.initialize(1, true);
    crate.set_firmware();
    crate.probe();
}

TEST_SUITE("xia::pixie::channel") {
    TEST_CASE("VOFFSET") {
        xia::pixie::sim::crate crate;
        setup_simulation(crate, 15, 16, 250, 16);
        const double expected_par = 0.77;
        const size_t expected_var = 49588;
        crate[0].write("VOFFSET", 0, expected_par);
        CHECK(crate[0].read_var("OffsetDAC", 0, 0) == expected_var);
        CHECK(doctest::Approx(crate[0].read("VOFFSET", 0)).epsilon(0.001) == expected_par);
    }
}