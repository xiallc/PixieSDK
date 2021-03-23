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
/// @file system_tests.hpp
/// @brief file that holds some commonly used functions for system tests.
/// @author S. V. Paulauskas
/// @date February 19, 2021

#ifndef PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP
#define PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP
#include <vector>
#include <string>

enum class DATA_PATTERN {
    HI_LO,
    FLIP_FLOP,
    RAMP_UP,
    RAMP_DOWN,
    CONSTANT,
    EVEN_BITS,
    ODD_BITS,
    ZERO
};

enum class DATA_IO {
    WRITE = 0,
    READ = 1
};

std::vector<unsigned int> prepare_data_to_write(const DATA_PATTERN& data_pattern,
                                                const unsigned int& size);

bool verify_api_return_value(const int& val, const std::string& func_name,
                             const bool& print_success_msg = true);

int verify_data_read(const unsigned int* expected, const unsigned int* returned,
                     const int& module_number, const unsigned int& size);

bool verify_power_of_two(const unsigned int& value);

#endif //PIXIE_SDK_SYSTEM_TEST_UTILITIES_HPP
