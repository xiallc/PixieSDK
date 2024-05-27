/*
 * Copyright 2024 XIA LLC, All rights reserved.
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

/** @file fw.cpp
 * @brief Implements functions and data structures related to firmware reporting
 */

#include <pixie/format.hpp>
#include <pixie/fw.hpp>

#include <pixie/reports/reports.hpp>

namespace xia {
namespace reports {
static void fill_fw_info(const pixie::firmware::firmware_set_ref& fw_set,
                         pixie::format::json& jfws) {
    pixie::format::json fw_set_info;
    fw_set_info["release"] = fw_set->release.to_string();
    fw_set_info["release_date"] = fw_set->release_date;
    fw_set_info["tag"] = fw_set->tag();
    for (auto fw : fw_set->firmwares) {
        pixie::format::json fw_file;
        fw_file["version"] = fw->version;
        fw_file["filename"] = fw->filename;
        fw_file["crc32"] = fw->crc;
        if (fw->device.name == "fippi") {
            fw_file["mask"] = fw->mask;
            fw_set_info["device"][fw->device.name].push_back(fw_file);
        } else {
            fw_set_info["device"][fw->device.name] = fw_file;
        }
    }
    jfws.push_back(fw_set_info);
}

static void fw_report(pixie::crate::crate& crate, pixie::format::json& jfws) {
    for (auto& fws : crate.firmware) {
        for (auto fw_set : std::get<1>(fws)) {
            fill_fw_info(fw_set, jfws);
        }
    }
}

void fw_report(pixie::crate::crate& crate, std::ostream& out) {
    pixie::format::json rpt;
    fw_report(crate, rpt);
    out << rpt;
}

void fw_report(pixie::crate::crate& crate, std::string& out) {
    pixie::format::json rpt;
    fw_report(crate, rpt);
    out = rpt.dump();
}

static void fw_report(pixie::crate::crate& crate, pixie::format::json& jfws,
    pixie::module::module& mod) {
    for (auto& fws : crate.firmware) {
        for (auto fw_set : std::get<1>(fws)) {
            if (fw_set->tag() == mod.get_fw_tag()) {
                fill_fw_info(fw_set, jfws);
            }
        }
    }
}

void fw_report(pixie::crate::crate& crate, std::ostream& out, pixie::module::module& mod) {
    pixie::format::json rpt;
    fw_report(crate, rpt, mod);
    out << rpt;
}

void fw_report(pixie::crate::crate& crate, std::string& out, pixie::module::module& mod) {
    pixie::format::json rpt;
    fw_report(crate, rpt, mod);
    out = rpt.dump();
}
}
}
