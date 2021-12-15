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

/** @file list_mode.cpp
 * @brief Defines classes and functions useful for list-mode data processing.
 */

#include <pixie/error.hpp>
#include <pixie/util.hpp>

#include <pixie/data/list_mode.hpp>

#include <nolhmann/json.hpp>

namespace xia {
namespace pixie {
namespace data {
namespace list_mode {

static void from_json(const nlohmann::json& j, event& evt) {
    j.at("cfd_forced_trigger").get_to(evt.cfd_forced_trigger);
    j.at("cfd_time").get_to(evt.cfd_time);
    j.at("cfd_trigger_source").get_to(evt.cfd_trigger_source);
    j.at("channel_number").get_to(evt.channel_number);
    j.at("crate_id").get_to(evt.crate_id);
    j.at("energy").get_to(evt.energy);
    j.at("energy_sums").get_to(evt.energy_sums);
    j.at("event_time_high").get_to(evt.event_time_high);
    j.at("event_time_low").get_to(evt.event_time_low);
    j.at("external_time_high").get_to(evt.external_time_high);
    j.at("external_time_low").get_to(evt.external_time_low);
    j.at("filter_baseline").get_to(evt.filter_baseline);
    j.at("finish_code").get_to(evt.finish_code);
    j.at("qdc").get_to(evt.qdc);
    j.at("slot_id").get_to(evt.slot_id);
    j.at("time").get_to(evt.time);
    j.at("trace_out_of_range").get_to(evt.trace_out_of_range);
    j.at("trace").get_to(evt.trace);
}

event::event()
    : cfd_forced_trigger(false), cfd_trigger_source(false), finish_code(false),
      trace_out_of_range(false), energy(0), filter_baseline(0), time(0), cfd_time(0),
      channel_number(0), crate_id(0), event_time_high(0), event_time_low(0), external_time_high(0),
      external_time_low(0), slot_id(0) {}

event::event(const std::string& json_string)
    : cfd_forced_trigger(false), cfd_trigger_source(false), finish_code(false),
      trace_out_of_range(false), energy(0), filter_baseline(0), time(0), cfd_time(0),
      channel_number(0), crate_id(0), event_time_high(0), event_time_low(0), external_time_high(0),
      external_time_low(0), slot_id(0) {

    try {
        nlohmann::json json = nlohmann::json::parse(json_string);
        from_json(json, *this);
    } catch (nlohmann::json::exception& ex) {
        throw xia::pixie::error::error(xia::pixie::error::code::invalid_value,
                                       "event constructor from JSON string: " +
                                           std::string(ex.what()));
    }
}

event::~event() = default;

bool event::operator==(const event& rhs) const {
    return crate_id == rhs.crate_id && slot_id == rhs.slot_id &&
           channel_number == rhs.channel_number && event_time_low == rhs.event_time_low &&
           event_time_high == rhs.event_time_high && energy == rhs.energy;
}

bool event::operator!=(const event& rhs) const {
    return !operator==(rhs);
}

bool event::operator<(const event& rhs) const {
    return raw_time() < rhs.raw_time();
}

bool event::operator>(const event& rhs) const {
    return !operator<(rhs);
}

uint64_t event::raw_time() const {
    return (uint64_t(event_time_high) << 32) | uint64_t(event_time_low);
}

uint64_t event::external_time() const {
    return (uint64_t(external_time_high) << 32) | uint64_t(external_time_low);
}

void event::output(std::ostream& out) const {
    util::ostream_guard flags(out);
    out << std::boolalpha << "crate: " << crate_id << " slot: " << slot_id
        << " channel: " << channel_number << " time: " << raw_time() << " energy: " << energy;
}

std::string event::as_json() const {
    nlohmann::json json = {{"cfd_forced_trigger", cfd_forced_trigger},
                           {"cfd_time", cfd_time},
                           {"cfd_trigger_source", cfd_trigger_source},
                           {"channel_number", channel_number},
                           {"crate_id", crate_id},
                           {"energy", energy},
                           {"energy_sums", energy_sums},
                           {"event_time_high", event_time_high},
                           {"event_time_low", event_time_low},
                           {"external_time_high", external_time_high},
                           {"external_time_low", external_time_high},
                           {"filter_baseline", filter_baseline},
                           {"finish_code", finish_code},
                           {"qdc", qdc},
                           {"slot_id", slot_id},
                           {"time", time},
                           {"trace", trace},
                           {"trace_out_of_range", trace_out_of_range}};
    return json.dump();
}

}  // namespace list_mode
}  // namespace data
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::data::list_mode::event& event) {
    event.output(out);
    return out;
}
