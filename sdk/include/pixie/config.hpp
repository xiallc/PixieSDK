#ifndef PIXIE_CONFIG_HPP
#define PIXIE_CONFIG_HPP
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
/// @file configuration.hpp
/// @brief Namespace and functions commonly used by the utilities programs.
/// @author S. V. Paulauskas
/// @date November 13, 2020

#include <string>

#include <pixie/error.hpp>
#include <pixie/os_compat.hpp>

#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
namespace config {

typedef pixie::error::error error;

struct configuration {
    int num_modules;
    module::number_slots slot_map;
    std::string com_fpga_config;
    std::string sp_fpga_config;
    std::string dsp_code;
    std::string dsp_param;
    std::string dsp_var;
};

PIXIE_EXPORT void PIXIE_API read(const std::string& config_file_name, configuration& cfg);

/*
 * Import a JSON confuguration into a crate's internal variables.
 */
void import_json(const std::string& filename,
                 crate::crate& crate,
                 module::number_slots& loaded);

/*
 * Export the crate's configuration to a JSON file.
 */
void export_json(const std::string& filename, crate::crate& crate);

}
}
}

#endif  // PIXIE_CONFIG_HPP
