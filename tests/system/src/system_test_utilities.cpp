/*----------------------------------------------------------------------
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
*----------------------------------------------------------------------*/
/// @file system_test_utilities.cpp
/// @brief Functions that are useful across several system test codes.
/// @author S. V. Paulauskas
/// @date February 26, 2021
#include "system_test_utilities.hpp"
#include "easylogging/easylogging++.h"

#include <iostream>

std::vector<unsigned int> prepare_data_to_write(const DATA_PATTERN& data_pattern,
                                                const unsigned int& size) {
    if (!verify_power_of_two(size))
        throw std::invalid_argument("Test data must have a size that's a power of 2!!");

    std::vector<unsigned int> data{};
    for (unsigned int entry = 0; entry < size; entry += 2) {
        switch (data_pattern) {
            case DATA_PATTERN::HI_LO:
                data.emplace_back(0xAAAA5555);
                data.emplace_back(0x5555AAAA);
                break;
            case DATA_PATTERN::FLIP_FLOP:
                data.emplace_back(0xA0500A05);
                data.emplace_back(0x50A0050A);
                break;
            case DATA_PATTERN::RAMP_UP:
                data.emplace_back(entry);
                data.emplace_back(entry + 1);
                break;
            case DATA_PATTERN::RAMP_DOWN:
                data.emplace_back(size - entry);
                data.emplace_back(size - entry - 1);
                break;
            case DATA_PATTERN::CONSTANT:
                data.emplace_back(0x50f750fa);
                data.emplace_back(0x50f750fa);
                break;
            case DATA_PATTERN::EVEN_BITS:
                data.emplace_back(0xA5A5A5A5);
                data.emplace_back(0xA5A5A5A5);
                break;
            case DATA_PATTERN::ODD_BITS:
                data.emplace_back(0x5A5A5A5A);
                data.emplace_back(0x5A5A5A5A);
                break;
            case DATA_PATTERN::ZERO:
                data.emplace_back(0);
                data.emplace_back(0);
                break;
            default:
                break;
        }
    }
    return data;
}

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success_msg /*=true*/) {
    if (val < 0) {
        LOG(ERROR) << func_name << " failed with Error Code " << val;
        return false;
    }
    if (print_success_msg)
        LOG(INFO) << func_name << " finished successfully.";
    return true;
}

int verify_data_read(const unsigned int* expected, const unsigned int* returned,
                     const int& module_number, const unsigned int& size) {
    unsigned int error_count = 0;
    for (unsigned int idx = 0; idx < size; idx++) {
        if (expected[idx] != returned[idx]) {
            LOG(ERROR) << " Data mismatch in module " << module_number << ", rd_data="
                       << std::hex << returned[idx] << ", wr_data=" << expected[idx]
                       << ", position=" << std::dec << idx << std::endl;
            error_count++;
        }
    }
    return error_count;
}

bool verify_power_of_two(const unsigned int& value) {
    if ((value != 0) && ((value & (value - 1)) == 0))
        return true;
    return false;
}
