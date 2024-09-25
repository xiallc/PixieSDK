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
#include <pixie/format.hpp>
#include <pixie/os_compat.h>

#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Tools for handling configuration files.
 */
namespace config {

using error = pixie::error::error;

/**
 * @brief Import a JSON configuration into a crate's internal variables.
 * @param[in] filename The name of the file with the relative or absolute path.
 * @param[in] crate The crate object that we're going to load this configuration into.
 * @param[in] loaded List of modules that will receive the configuration.
 */
void import_settings_file(const std::string& filename, crate::crate& crate,
    module::number_slots& loaded);

/**
 * @brief Import a JSON configuration for a module
 * @param[in] filename The name of the file with the relative or absolute path.
 * @param[in] mod The module object that we're going to load this configuration into.
 */
void import_settings_file(const std::string& filename, module::module& mod);

/**
 * @brief Import a JSON configuration into a crate's internal variables.
 * @param[in] config The JSON string to import.
 * @param[in] crate The crate object that we're going to load this configuration into.
 * @param[in] loaded List of modules that will receive the configuration.
 */
void import_settings(std::string& config, crate::crate& crate, module::number_slots& loaded);

/**
 * @brief Import a JSON configuration for a module.
 * @param[in] config The JSON string to import.
 * @param[in] mod The module object that we're going to load this configuration into.
 */
void import_settings(std::string& config, module::module& mod);

/**
 * @brief Export the active module configurations to a JSON file.
 * @param[in] filename The name of the JSON output file used for the export.
 * @param[in] crate The crate object holding the modules to be exported.
 */
void export_settings_file(const std::string& filename, crate::crate& crate);

/**
 * @brief Export the active module configurations in JSON format.
 * @param[in] config The string the JSON object data is exported to.
 * @param[in] crate The crate object holding the modules to be exported.
 */
void export_settings(std::string& config, crate::crate& crate);

}  // namespace config
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_CONFIG_HPP
