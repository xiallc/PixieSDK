#ifndef PIXIE16_ERROR_H
#define PIXIE16_ERROR_H

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

#include <iostream>
#include <stdexcept>
#include <sstream>

namespace xia
{
namespace pixie
{
namespace error
{
/*
 * Codes
 */
enum struct code {
    /*
     * Success is always 0. Do not number any other enum entries.
     */
    success = 0,
    /*
     * Crate
     */
    crate_already_open,
    crate_not_ready,
    crate_invalid_param,
    /*
     * Module
     */
    module_number_invalid,
    module_total_invalid,
    module_already_open,
    module_close_failure,
    module_offline,
    module_info_failure,
    module_invalid_operation,
    module_invalid_firmware,
    module_initialize_failure,
    module_invalid_param,
    module_invalid_var,
    module_task_timeout,
    /*
     * Channel
     */
    channel_number_invalid,
    channel_invalid_param,
    channel_invalid_var,
    /*
     * Device
     */
    device_load_failure,
    device_boot_failure,
    device_initialize_failure,
    device_copy_failure,
    device_image_failure,
    device_hw_failure,
    device_dma_failure,
    device_dma_busy,
    /*
     * File handling
     */
    file_not_found,
    file_read_failure,
    file_size_invalid,
    file_create_failure,
    /*
     * System
     */
    no_memory,
    slot_map_invalid,
    invalid_value,
    /*
     * Catch all
     */
    unknown_error,
    internal_failure,
    bad_error_code,
    last
};

/*
 * All exceptions need to be based on this error.
 */
struct error
    : public std::runtime_error {
    typedef pixie::error::code code;
    const code type;

    explicit error(const code type, const std::ostringstream& what);
    explicit error(const code type, const std::string& what);
    explicit error(const code type, const char* what);
    virtual ~error() = default;

    virtual void output(std::ostream& out);

    int result() const;
    std::string result_text() const;

    int return_code() const;
};

/*
 * Get the API code for an error code.
 */
int api_result(enum code type);
std::string api_result_text(enum code type);

/*
 * Unknown error helper.
 */
int api_result_unknown_error();
}
}
}

std::ostringstream& operator<<(std::ostringstream& out, xia::pixie::error::error& error);
std::ostream& operator<<(std::ostream& out, xia::pixie::error::error& error);

#endif  // PIXIE16_ERROR_H
