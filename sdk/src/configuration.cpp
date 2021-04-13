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
/// @file configuration.cpp
/// @brief Implementation for the configuration header
/// @author S. V. Paulauskas
/// @date April 13, 2021

#include "configuration.hpp"
namespace xia {
namespace config {
Configuration read_configuration_file(const std::string& config_file_name) {
    std::ifstream input(config_file_name, std::ios::in);

    if (input.fail()) {
        std::stringstream errmsg;
        errmsg << "Could not open " << config_file_name << "!";
        throw std::invalid_argument(errmsg.str());
    }

    xia::config::Configuration cfg;
    input >> cfg.numModules;
    cfg.slot_map = new unsigned short[cfg.numModules + 1];
    for (size_t i = 0; i < cfg.numModules; i++)
        input >> cfg.slot_map[i];

    input >> cfg.ComFPGAConfigFile >> cfg.SPFPGAConfigFile >> cfg.TrigFPGAConfigFile >>
          cfg.DSPCodeFile >> cfg.DSPParFile >> cfg.DSPVarFile;

    input.close();
    return cfg;
}
}  // namespace configuration
}  // namespace xia
