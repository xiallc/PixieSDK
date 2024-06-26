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

/** @file reports.hpp
 * @brief Defines functions and data structures related to handling reporting
 */

#ifndef PIXIESDK_REPORTS_HPP
#define PIXIESDK_REPORTS_HPP

#include <iostream>

#include <pixie/format.hpp>
#include <pixie/param.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/db/db.hpp>
#include <pixie/pixie16/fixture.hpp>
#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/module.hpp>

using error = xia::pixie::error::error;

namespace xia {
namespace reports {
/**
 * A report for a hardware configuration object
 * @param config The hardware configuration to read from
 * @param out The stream to write the report to
 * @param prefix
 */
void report(pixie::hw::config config, std::ostream& out, const std::string& prefix = "");
/**
 * A report for a channel fixture object
 * @param channel The channel fixture to read from
 * @param out The stream to write the report to
 * @param prefix
 */
void report(pixie::fixture::channel& channel, std::ostream& out, const std::string& prefix = "");
/**
 * A report for a daughter board fixture object
 * @param db The daughter board fixture to read from
 * @param out The stream to write the report to
 * @param prefix
 */
void report(pixie::fixture::db::db& db, std::ostream& out, const std::string& prefix = "");
/**
 * A report for a channel object
 * @param channel The channel to read from
 * @param out The stream to write the report to
 */
void report(pixie::channel::channel& channel, std::ostream& out);
/**
 * A report for a module object
 * @param module The module to read from
 * @param out The stream to write the report to
 */
void report(pixie::module::module& module, std::ostream& out);
/**
 * A report for a module object
 * @param module The module to read from
 * @param out The JSON object to populate the report to
 */
void report(pixie::module::module& module, pixie::format::json& out);
/**
 * A report for a crate object
 * @param crate The crate to read from
 * @param out The stream to write the report to
 */
void report(pixie::crate::crate& crate, std::ostream& out);
/**
 * A report for a crate object
 * @param crate The crate to read from
 * @param out The JSON object to populate the report to
 */
void report(pixie::crate::crate& crate, pixie::format::json& out);
/**
 * A report for a crate object
 * @param crate The crate to read from
 * @param out The stream to write the report to
 * @param slots The slots in the crate to report on. If empty, report on all slots
 */
void report(pixie::crate::crate& crate, std::ostream& out, pixie::hw::slot_numbers& slots);
/**
 * A report for a crate object
 * @param crate The crate to read from
 * @param out The JSON object to populate the report to
 * @param slots The slots in the crate to report on. If empty, report on all slots
 */
void report(pixie::crate::crate& crate, pixie::format::json& out, pixie::hw::slot_numbers& slots);
/**
 * A report for a module's stats object
 * @param module The module to read stats from
 * @param out The stream to write the report to
 */
void stats_report(pixie::module::module& module, std::ostream& out);
/**
 * A report for a crate's firmware
 * @param crate The crate to report on
 * @param out The stream to write the report to
 */
void fw_report(pixie::crate::crate& crate, std::ostream& out);
/**
 * A report for a crate's firmware
 * @param crate The crate to report on
 * @param out The stream to write the report to
 */
void fw_report(pixie::crate::crate& crate, std::string& out);
/**
 * A report for a crate's firmware
 * @param crate The crate holding the firmware
 * @param out The stream to write the report to
 * @param module The module to report on
 */
void fw_report(pixie::crate::crate& crate, std::ostream& out, pixie::module::module& mod);
/**
 * A report for a crate's firmware
 * @param crate The crate holding the firmware
 * @param out The stream to write the report to
 * @param module The module to report on
 */
void fw_report(pixie::crate::crate& crate, std::string& out, pixie::module::module& mod);

/*
 * Serialises a JSON object into std::string
 */
template <class T>
void report(T& obj, std::string& out) {
    pixie::format::json json_rpt;
    report(obj, json_rpt);
    out = json_rpt.dump();
}

/*
 * Serialises a JSON object into std::string for chosen slots
 */
template <class T>
void report(T& obj, std::string& out, pixie::hw::slot_numbers& slots) {
    pixie::format::json json_rpt;
    report(obj, json_rpt, slots);
    out = json_rpt.dump();
}

/**
 * Used to iterate through and report on a set of variables/parameters. The index is used
 * by channel variables and unused by module variables.
 */
template <class T>
void var_report(T& vars, std::ostream& out, int index = -1) {
    for (auto& var : vars) {
        auto address = var.var.address;
        if (index >= 0) {
            address += index;
        }
        std::ostringstream vartitle;
        vartitle << var.var.name;
        out << vartitle.str() << std::endl
            << std::string(vartitle.str().length(), '-') << std::endl
            << "Mode           : " << pixie::param::label(var.var.mode) << std::endl
            << "Access         : " << pixie::param::label(var.var.state) << std::endl
            << std::hex << std::setfill('0') << "Address        : 0x" << std::setw(8)
            << address;
        if (index >= 0) {
            out << " (0x" << var.var.address << " + " << index << ')';
        }
        out << std::endl
            << std::dec << std::setfill(' ')
            << "Size           : " << var.var.size << std::endl
            << "Index          : " << int(var.var.par) << std::endl
            << std::endl;
    }
}
}
}

#endif  //PIXIESDK_REPORTS_HPP
