/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

/** @file crate.hpp
 * @brief Onmitool Crate commands
 */

#ifndef ONMITOOL_CRATE_HPP
#define ONMITOOL_CRATE_HPP

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace crate {
/**
 * Register the commands.
 */
void register_commands();
} // namespace crate
} // namespace omnitool
} // namespace xia

#endif  // ONMITOOL_CRATE_HPP
