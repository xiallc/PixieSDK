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

/** @file config.hpp
 * @brief Defines data structures and functions for working with SDK configuration files.
 */

#ifndef PIXIE_CONFIG_HPP
#define PIXIE_CONFIG_HPP

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
void import_json(const std::string& filename, crate::crate& crate, module::number_slots& loaded);

/*
 * Export the crate's configuration to a JSON file.
 */
void export_json(const std::string& filename, crate::crate& crate);

}  // namespace config
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_CONFIG_HPP
