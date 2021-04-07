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
#include <iomanip>
#include <map>

#include <pixie_error.hpp>
#include <pixie_log.hpp>
#include <pixie_util.hpp>

namespace xia
{
namespace pixie
{
namespace error
{
struct result_code
{
    const int result;
    const char* text;

    result_code()
        : result(99),
          text("error code not set") {
    }
    result_code(const int result_, const char* text_)
        : result(result_),
          text(text_) {
    }
};

static std::map<code, result_code> result_codes =
{
    { code::success,                   {  0, "success" } },
    { code::module_number_invalid,     { 100, "invalid module number" } },
    { code::module_total_invalid,      { 101, "invalid module count" } },
    { code::module_alread_open,        { 102, "module already open" } },
    { code::module_close_failure,      { 103, "module failed to close" } },
    { code::module_info_failure,       { 104, "module information failure" } },
    { code::module_invalid_operation,  { 105, "invalid module operation" } },
    { code::module_invalid_firmware,   { 106, "invalid module firmware" } },
    { code::module_initialize_failure, { 107, "module initialization failure" } },
    { code::device_load_failure,       { 700, "device failed to load" } },
    { code::device_boot_failure,       { 701, "device failed to boot" } },
    { code::device_initialize_failure, { 702, "device failed to initialize" } },
    { code::device_copy_failure,       { 703, "device variable copy failed" } },
    { code::file_not_found,            { 704, "file not found" } },
    { code::file_read_failure,         { 705, "file read failure" } },
    { code::file_size_invalid,         { 706, "invalid file size" } },
    { code::file_create_failure,       { 707, "file create failure" } },
    { code::no_memory,                 { 800, "no memory" } },
    { code::slot_map_invalid,          { 801, "invalid slot map" } },
    { code::internal_failure,          { 900, "internal failure" } },
    { code::bad_error_code,            { 990, "bad error code" } },
};

error::error(const code type_, const std::ostringstream& what)
    : runtime_error(what.str()),
      type(type_)
{
    log(log::debug) << "error-except: " << what.str();
}

error::error(const code type_, const std::string& what)
    : runtime_error(what),
      type(type_)
{
    log(log::debug) << "error-except: " << what;
}

error::error(const code type_, const char* what)
    : runtime_error(what),
      type(type_)
{
    log(log::debug) << "error-except: " << what;
}

void
error::output(std::ostream& out)
{
    ostream_guard flags(out);
    out << std::setfill(' ')
        << "error: code:"
        << std::setw(2) << static_cast<int>(type)
        << " : " << what();
}

int
error::result() const
{
    return api_result(type);
}

std::string
error::result_text() const
{
    return api_result_text(type);
}

int
error::return_code() const
{
    return 0 - result();
}

int
api_result(enum code type)
{
    auto search = result_codes.find(type);
    int result;
    if (search == result_codes.end()) {
        result = result_codes[code::bad_error_code].result;
    } else {
        result = search->second.result;
    }
    return result;
}

std::string
api_result_text(enum code type)
{
    auto search = result_codes.find(type);
    std::string text;
    if (search == result_codes.end()) {
        text = result_codes[code::bad_error_code].text;
    } else {
        text = search->second.text;
    }
    return text;
}

}
}
}

std::ostream&
operator<<(std::ostream& out, xia::pixie::error::error& error)
{
    error.output(out);
    return out;
}
