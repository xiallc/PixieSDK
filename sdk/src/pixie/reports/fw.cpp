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

void fw_report(pixie::crate::crate& crate, std::ostream& out) {
    pixie::format::json jfws = pixie::format::json::array();
    for (auto& fws : crate.firmware) {
        for (auto fw_set : std::get<1>(fws)) {
            pixie::format::json fw_set_info;
            fw_set_info["release"] = fw_set->release.to_string();
            fw_set_info["release-data"] = fw_set->release_date;
            fw_set_info["tag"] = fw_set->tag();
            pixie::format::json jfw = pixie::format::json::array();
            for (auto fw : fw_set->firmwares) {
                pixie::format::json fw_info;
                fw_info["tag"] = fw->tag;
                fw_info["version"] = fw->version;
                fw_info["mod-revision"] = fw->device.mod_revision;
                fw_info["adc-msps"] = fw->device.mod_adc_msps;
                fw_info["adc-bits"] = fw->device.mod_adc_bits;
                fw_info["device"] = fw->device.name;
                fw_info["file"] = fw->filename;
                fw_info["crc32"] = fw->crc;
                jfw.push_back(fw_info);
            }
            jfws.push_back(fw_set_info);
        }
    }
    out << jfws;
}
}
}
