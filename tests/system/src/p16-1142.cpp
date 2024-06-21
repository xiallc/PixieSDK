/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file p16_246.cpp
 * @brief Tests the system when we allow the external FIFO to fill up.
 *
 * For now, we've hard coded the pathing for our Linux test machine. This test
 * isn't really intended to be run anywhere else for the moment. We'll clean up
 * the pathing at a later date.
 */
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "pixie16/pixie16.h"

int main() {
    std::vector<unsigned short> slot_map = {10, 9, 13};
    std::string output_settings = "./p16-1142_output.set";
    std::string log_name = "./p16-1142_output.log";

    bool use_a = true;
    if (use_a) {
        slot_map = {13, 10, 9};
        output_settings = "./p16-1142_output-a.set";
        log_name = "./p16-1142_output-a.log";
    }

    std::string input_settings = "./p16-1142_input.set";
    int retval;

    retval = Pixie16InitSystem(static_cast<unsigned short>(slot_map.size()), slot_map.data(), 0);
    std::cout << "Pixie16InitSystem: " << retval << std::endl;

    // Slot 10 : 17-250-14
    auto slot10_pos = static_cast<unsigned short>(
        std::distance(slot_map.begin(), std::find(slot_map.begin(), slot_map.end(), 10)));
    retval = Pixie16BootModule(
        "/usr/local/xia/pixie-16/firmware/17-250-14/75/0.11.0/sys.bin",
        "/usr/local/xia/pixie-16/firmware/17-250-14/75/0.11.0/fippi.bin", nullptr,
        "/usr/local/xia/pixie-16/firmware/17-250-14/75/0.11.0/dsp.ldr", input_settings.c_str(),
        "/usr/local/xia/pixie-16/firmware/17-250-14/75/0.11.0/dsp.var", slot10_pos, 0x7F);
    std::cout << "Pixie16BootModule(" << slot10_pos << ") - Slot 10: " << retval << std::endl;

    // Slot 9 : 15-250-14
    auto slot9_pos = static_cast<unsigned short>(
        std::distance(slot_map.begin(), std::find(slot_map.begin(), slot_map.end(), 9)));

    retval = Pixie16BootModule(
        "/usr/local/xia/pixie-16/firmware/15-250-14/75/1.0.1/sys.bin",
        "/usr/local/xia/pixie-16/firmware/15-250-14/75/1.0.1/fippi.bin", nullptr,
        "/usr/local/xia/pixie-16/firmware/15-250-14/75/1.0.1/dsp.ldr", input_settings.c_str(),
        "/usr/local/xia/pixie-16/firmware/15-250-14/75/1.0.1/dsp.var", slot9_pos, 0x7F);
    std::cout << "Pixie16BootModule(" << slot9_pos << ") - Slot 9: " << retval << std::endl;

    // Slot 13 : 12-100-12
    auto slot13_pos = static_cast<unsigned short>(
        std::distance(slot_map.begin(), std::find(slot_map.begin(), slot_map.end(), 13)));
    retval = Pixie16BootModule(
        "/usr/local/xia/pixie-16/firmware/13-100-12/75/0.9.1/sys.bin",
        "/usr/local/xia/pixie-16/firmware/13-100-12/75/0.9.1/fippi.bin", nullptr,
        "/usr/local/xia/pixie-16/firmware/13-100-12/75/0.9.1/dsp.ldr", input_settings.c_str(),
        "/usr/local/xia/pixie-16/firmware/13-100-12/75/0.9.1/dsp.var", slot13_pos, 0x7F);
    std::cout << "Pixie16BootModule(" << slot13_pos << ") - Slot 13: " << retval << std::endl;

    retval = Pixie16SaveDSPParametersToFile(output_settings.c_str());
    std::cout << "Pixie16SaveDSPParametersToFile: " << retval << std::endl;

    retval = Pixie16ExitSystem(static_cast<unsigned short>(slot_map.size()));
    std::cout << "Pixie16ExitSystem: " << retval << std::endl;

    std::stringstream mv_log_cmd;
    mv_log_cmd << "mv Pixie16Msg.log " << log_name;
    return system(mv_log_cmd.str().c_str());
}
