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

/** @file fixture.cpp
 * @brief Implements functions and data structures related to fixture reporting
 */

#include <pixie/reports/reports.hpp>
#include <pixie/utils/io.hpp>

namespace xia {
namespace reports {

void report(pixie::fixture::channel& fixture, std::ostream& out, const std::string& prefix) {
    out << prefix << "Fixture        : " << fixture.label << std::endl;
    report(fixture.config, out, prefix);
}

void report(pixie::fixture::db::db db, std::ostream& out, const std::string& prefix) {
    report((pixie::fixture::channel&) db, out, prefix);
    out << prefix << "DB Number      : " << db.number.get<int>() << std::endl
        << prefix << "DB Base        : " << db.base.get<int>() << std::endl
        << prefix << "DB Channels    : " << db.channels.get<int>() << std::endl;
}

}
}
