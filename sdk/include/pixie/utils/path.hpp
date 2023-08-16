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

/** @file fileutils.hpp
 * @brief Defines utility functions and data structures related to filepaths useful throughout the SDK.
 */

#ifndef PIXIESDK_UTIL_FILE_HPP
#define PIXIESDK_UTIL_FILE_HPP

#include <string>
#include <vector>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace path {

using strings = std::vector<std::string>;

/**
 * @brief Look for a file at the given path and store the filename
 *
 * @param path The filepath as a string
 * @param files A string vector to store filenames in
 * @param ext The type of file extension as a string
 * @param depth The depth at which the deepest file found is
*/
void find_files(const std::string path, strings& files_, const std::string& ext, size_t depth = 0);

/**
 * @brief Returns the base filename of a given filename string
*/
const std::string basename(const std::string& name);

/**
 * @brief Returns the directory tree of a given filename as a filepath string
*/
const std::string dirname(const std::string& name);

/**
 * @brief Returns the extension for the given filepath
*/
const std::string extension(const std::string& name);

} // namespace path
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_FILE_HPP
