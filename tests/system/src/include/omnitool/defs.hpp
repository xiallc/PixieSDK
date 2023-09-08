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

/** @file defs.hpp
 * @brief Onmitool Definitions
 */

#ifndef ONMITOOL_DEFS_HPP
#define ONMITOOL_DEFS_HPP

namespace xia {
namespace omnitool {
/**
 * File prefixes
 */
static constexpr const char* adc_prefix = "pixie16-omnitool-adc";
static constexpr const char* histogram_prefix = "pixie16-omnitool-mca";
static constexpr const char* baseline_prefix = "pixie16-omnitool-baseline";
static constexpr const char* list_mode_prefix = "pixie16-omnitool-list-mode";
} // namespace omnitool
} // namespace xia

#endif  // ONMITOOL_DEFS_HPP
