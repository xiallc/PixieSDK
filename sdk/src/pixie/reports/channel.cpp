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

/** @file channel.cpp
 * @brief Implements functions and data structures related to channel reporting
 */

#include <pixie/reports/reports.hpp>
#include <pixie/utils/io.hpp>

namespace xia {
namespace reports {

void report(pixie::channel::channel& channel, std::ostream& out) {
    std::ostringstream channeltitle;
    channeltitle << "Channel " << channel.number;
    out << channeltitle.str() << std::endl
        << std::string(channeltitle.str().length(), '~') << std::endl
        << "Number         : " << channel.number << std::endl;
    report(*channel.fixture, out);
    out << std::endl;
    var_report(channel.vars, out, channel.fixture->config.index);
}

}
}