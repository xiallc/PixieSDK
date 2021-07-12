#ifndef PIXIE_PIXIE16_LEGACY_HPP
#define PIXIE_PIXIE16_LEGACY_HPP
/**----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
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
/// @file lagacy.hpp
/// @brief Namespace and functions to interface with lgeacy data
/// @author Chris Johns
/// @date 9 Jul 2021

#include <string>

#include <pixie/error.hpp>
#include <pixie/os_compat.hpp>

#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
namespace legacy {

typedef pixie::error::error error;

/*
 * Legacy Settings configuration data.
 */
struct settings {
    /*
     * Hardcoded constant from the legacy code.
     */
    static const size_t N_DSP_PAR = 1280;

    /*
     * Legacy settings files are limited to 16 channels only
     */
    static const size_t MAX_CHANNELS = 16;

    /*
     * The number of DSP words written when pushing the variables as a block.
     */
    static const size_t DSP_IO_BORDER = 832;

    hw::words dsp_mem;
    param::module_var_descs module_var_descriptors;
    param::channel_var_descs channel_var_descriptors;
    param::address_map addresses;

    /*
     * Construct the parameters and memory map from a var file or any existing
     * module. Must be a 16 channel module.
     */
    settings(firmware::firmware_ref vars);
    settings(module::module& module);

    int num_modules() const;

    /*
     * Load the settings file into the DSP memory buffer
     */
    void load(const std::string& parfile);

    /*
     * Import the DSP memory contents into the module
     */
    void import(module::module& module);

    /*
     * Write the DSP memory contents to the DSP hardware
     */
    void write(module::module& module);

    /*
     * Read a var value from the internal DSP memory buffer
     */
    param::value_type read_var(param::module_var var,
                               int module,
                               size_t offset = 0) const;
    param::value_type read_var(param::channel_var var,
                               int module,
                               int channel,
                               size_t offset = 0) const;

    /*
     * Values of interest when looking at par files.
     */
    size_t index(const param::module_var_desc& desc,
                 int module,
                 size_t offset = 0) const;
    size_t index(const param::channel_var_desc& desc,
                 int module,
                 int channel,
                 size_t offset = 0) const;
    size_t address(const param::module_var_desc& desc,
                   size_t offset = 0) const;
    size_t address(const param::channel_var_desc& desc,
                   int channel,
                   size_t offset = 0) const;

    /*
     * Output the settings file contents in a readable format
     */
    void output(std::ostream& out) const;
};

}
}
}

/*
 * Output stream operator.
 */
std::ostream&
operator<<(std::ostream& out, const xia::pixie::legacy::settings& settings);

#endif  // PIXIE_PIXIE16_LEGACY_HPP
