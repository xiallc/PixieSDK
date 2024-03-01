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

#include <pixie/utils/string.hpp>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace path {

using paths = string::strings;

/**
 * @brief Path separattor
 */
extern const char path_sep;

/**
 * @brief Look for a file at the given path and store the filename
 *
 * @param path The filepath as a string
 * @param files A string vector to store filenames in
 * @param ext The type of file extension as a string
 * @param ignore_error If true ignore any error
 * @param depth The depth at which the deepest file found is
*/
void find_files(const std::string path, paths& files_, const std::string& ext,
                bool ignore_error = false, size_t depth = 0);

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

/**
 * @brief Returns the path and paths joined as a single path. If path
 * is empty the path separator is prepended and if the last paths
 * element is empty the path separator is appended. If a paths element
 * is absolute the path is generated from that element to the end and
 * path argument is ignored.
 *
 * An absolute path on Windows is `d:` and on POSIX `/`.
 *
 * This call is similar to Python's os.path.join() call.
 *
 * @param path A path prefixed to the joined path
 * @param paths A list of paths join together
 * @return The joined path
 */
const std::string join(const std::string path, const paths& paths);

/**
 * @brief Returns true if the path is valid and exists
*/
bool exists(const std::string& path);

/**
 * @brief Returns true if the path is a file
*/
bool isfile(const std::string& path);

/**
 * @brief Returns true if the path is a directory
*/
bool isdir(const std::string& path);

} // namespace path
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_FILE_HPP
