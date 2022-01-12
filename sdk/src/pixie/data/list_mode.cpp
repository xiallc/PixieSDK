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

static const size_t num_qdc_words = 8;
static const size_t num_esum_words = 4;
static const size_t num_ext_ts_words = 2;

static void from_json(const nlohmann::json& j, event& evt) {
    j.at("cfd_forced_trigger").get_to(evt.cfd_forced_trigger);
    j.at("cfd_fractional_time").get_to(evt.cfd_fractional_time);
    j.at("cfd_trigger_source").get_to(evt.cfd_trigger_source);
    j.at("channel_number").get_to(evt.channel_number);
    j.at("crate_id").get_to(evt.crate_id);
    j.at("energy").get_to(evt.energy);
    j.at("energy_sums").get_to(evt.energy_sums);
    j.at("event_length").get_to(evt.event_length);
    j.at("event_time_high").get_to(evt.event_time_high);
    j.at("event_time_low").get_to(evt.event_time_low);
    j.at("external_time_high").get_to(evt.external_time_high);
    j.at("external_time_low").get_to(evt.external_time_low);
    j.at("filter_baseline").get_to(evt.filter_baseline);
    j.at("finish_code").get_to(evt.finish_code);
    j.at("header_length").get_to(evt.header_length);
    j.at("qdc").get_to(evt.qdc);
    j.at("slot_id").get_to(evt.slot_id);
    j.at("time").get_to(evt.time);
    j.at("trace_out_of_range").get_to(evt.trace_out_of_range);
    j.at("trace").get_to(evt.trace);
}

event::event()
    : cfd_forced_trigger(false), cfd_fractional_time(0), cfd_trigger_source(0), channel_number(0),
      crate_id(0), energy(0), event_length(0), event_time_high(0), event_time_low(0),
      external_time_high(0), external_time_low(0), filter_baseline(0), finish_code(false),
      header_length(0), slot_id(0), time(0), trace_length(0), trace_out_of_range(false) {}

event::event(const std::string& json_string)
    : cfd_forced_trigger(false), cfd_fractional_time(0), cfd_trigger_source(0), channel_number(0),
      crate_id(0), energy(0), event_length(0), event_time_high(0), event_time_low(0),
      external_time_high(0), external_time_low(0), filter_baseline(0), finish_code(false),
      header_length(0), slot_id(0), time(0), trace_length(0), trace_out_of_range(false) {

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

std::string event_as_json(const event& evt) {
    nlohmann::json json = {{"cfd_forced_trigger", evt.cfd_forced_trigger},
                           {"cfd_fractional_time", evt.cfd_fractional_time},
                           {"cfd_trigger_source", evt.cfd_trigger_source},
                           {"channel_number", evt.channel_number},
                           {"crate_id", evt.crate_id},
                           {"energy", evt.energy},
                           {"energy_sums", evt.energy_sums},
                           {"event_length", evt.event_length},
                           {"event_time_high", evt.event_time_high},
                           {"event_time_low", evt.event_time_low},
                           {"external_time_high", evt.external_time_high},
                           {"external_time_low", evt.external_time_high},
                           {"filter_baseline", evt.filter_baseline},
                           {"finish_code", evt.finish_code},
                           {"header_length", evt.header_length},
                           {"qdc", evt.qdc},
                           {"slot_id", evt.slot_id},
                           {"time", evt.time},
                           {"trace", evt.trace},
                           {"trace_out_of_range", evt.trace_out_of_range}};
    return json.dump();
}

/**
 * @brief A structure defining the information regarding a data mask.
 */
struct element_desc {
    /**
     * @brief The element type that we're describing.
     */
    const element type;
    /**
     * @brief Defines the value of the descriptor
     */
    const size_t value;
    /**
     * @brief The bit that the value occupies within the data structure
     */
    const size_t start_bit;
    /**
     * @brief The header word that this element appears on.
     */
    const size_t header_index;

    element_desc(const element type_, const size_t value_, const size_t bit_,
                 const size_t header_index_)
        : type(type_), value(value_), start_bit(bit_), header_index(header_index_) {}
};

static const std::vector<element_desc> descriptors_17562_100 = {
    element_desc(element::cfd_fractional_time, 0xFFFF0000, 16, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x3FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x40000000, 30, 0)};

static const std::vector<element_desc> descriptors_29432_100 = {
    element_desc(element::cfd_fractional_time, 0xFFFF0000, 16, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x00007FFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x00008000, 15, 3)};

static const std::vector<element_desc> descriptors_30474_100 = {
    element_desc(element::cfd_forced_trigger_bit, 0x80000000, 31, 2),
    element_desc(element::cfd_fractional_time, 0x7FFF0000, 16, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x00007FFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x00008000, 15, 3)};

static const std::vector<element_desc> descriptors_34688_100 = {
    element_desc(element::cfd_forced_trigger_bit, 0x80000000, 31, 2),
    element_desc(element::cfd_fractional_time, 0x7FFF0000, 16, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0x7FFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x80000000, 31, 3)};

static const std::vector<element_desc> descriptors_20466_250 = {
    element_desc(element::cfd_fractional_time, 0xFFFF0000, 16, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x3FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x40000000, 30, 0)};

static const std::vector<element_desc> descriptors_27361_250 = {
    element_desc(element::cfd_fractional_time, 0x7FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0x80000000, 31, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x3FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x40000000, 30, 0),
};

static const std::vector<element_desc> descriptors_29432_250 = {
    element_desc(element::cfd_fractional_time, 0x7FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0x80000000, 31, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x00007FFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x00008000, 15, 3)};

static const std::vector<element_desc> descriptors_30474_250 = {
    element_desc(element::cfd_forced_trigger_bit, 0x80000000, 31, 2),
    element_desc(element::cfd_fractional_time, 0x3FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0x40000000, 30, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x00007FFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x00008000, 15, 3)};

static const std::vector<element_desc> descriptors_34688_250 = {
    element_desc(element::cfd_forced_trigger_bit, 0x80000000, 31, 2),
    element_desc(element::cfd_fractional_time, 0x3FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0x40000000, 30, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0x7FFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x80000000, 31, 3)};

static const std::vector<element_desc> descriptors_46540_250 = {
    element_desc(element::cfd_forced_trigger_bit, 0x80000000, 31, 2),
    element_desc(element::cfd_fractional_time, 0x3FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0x40000000, 30, 2),
    element_desc(element::channel_number, 0x0000003F, 0, 0),
    element_desc(element::crate_id, 0x00000C00, 10, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000003C0, 6, 0),
    element_desc(element::trace_length, 0x7FFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x80000000, 31, 3)};

static const std::vector<element_desc> descriptors_29432_500 = {
    element_desc(element::cfd_fractional_time, 0x1FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0xE0000000, 29, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x00007FFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0xFFFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x00008000, 15, 3)};

static const std::vector<element_desc> descriptors_34688_500 = {
    element_desc(element::cfd_fractional_time, 0x1FFF0000, 16, 2),
    element_desc(element::cfd_trigger_source_bit, 0xE0000000, 29, 2),
    element_desc(element::channel_number, 0x0000000F, 0, 0),
    element_desc(element::crate_id, 0x00000F00, 8, 0),
    element_desc(element::energy, 0x0000FFFF, 0, 3),
    element_desc(element::event_length, 0x7FFE0000, 17, 0),
    element_desc(element::event_time_high, 0x0000FFFF, 0, 2),
    element_desc(element::event_time_low, 0xFFFFFFFF, 0, 1),
    element_desc(element::finish_code, 0x80000000, 31, 0),
    element_desc(element::header_length, 0x0001F000, 12, 0),
    element_desc(element::slot_id, 0x000000F0, 4, 0),
    element_desc(element::trace_length, 0x7FFF0000, 16, 3),
    element_desc(element::trace_out_of_range_flag, 0x80000000, 31, 3)};

static std::vector<element_desc> find_element_set(size_t rev, size_t freq) {
    if (rev < 17562)
        throw error(error::code::invalid_revision, "minimum supported firmware rev is 17562.");

    switch (freq) {
        case 100:
            if (rev >= 17562 && rev < 29432) {
                return descriptors_17562_100;
            } else if (rev >= 29432 && rev < 30474) {
                return descriptors_29432_100;
            } else if (rev >= 30474 && rev < 34688) {
                return descriptors_30474_100;
            } else {
                return descriptors_34688_100;
            }
        case 250:
            if (rev < 20466) {
                throw error(error::code::invalid_revision,
                            "minimum supported firmware rev is 20466.");
            } else if (rev >= 20466 && rev < 27361) {
                return descriptors_20466_250;
            } else if (rev >= 27361 && rev < 29432) {
                return descriptors_27361_250;
            } else if (rev >= 29432 && rev < 30474) {
                return descriptors_29432_250;
            } else if (rev >= 30474 && rev < 34688) {
                return descriptors_30474_250;
            } else if (rev >= 34688 && rev < 46540) {
                return descriptors_34688_250;
            } else {
                return descriptors_46540_250;
            }
        case 500:
            if (rev < 29432) {
                throw error(error::code::invalid_revision,
                            "minimum supported firmware rev is 29432.");
            } else if (rev >= 29432 && rev < 34688) {
                return descriptors_29432_500;
            } else {
                return descriptors_34688_500;
            }
        default:
            throw error(error::code::invalid_frequency,
                        "invalid frequency: " + std::to_string(freq));
    }
}

events decode_data_block(uint32_t* data, const size_t len, const size_t revision,
                         const size_t frequency) {
    events evts;
    uint32_t* data_start = data;

    if (len < 4) {
        throw error(error::code::invalid_header_length, "minimum data buffer size is 4");
    }

    auto core_elements = find_element_set(revision, frequency);

    while (data < data_start + len) {
        event evt;

        for (const auto& ele : core_elements) {
            auto val = (data[ele.header_index] & ele.value) >> ele.start_bit;
            switch (ele.type) {
                case element::cfd_forced_trigger_bit:
                    evt.cfd_forced_trigger = val != 0;
                    break;
                case element::cfd_fractional_time:
                    evt.cfd_fractional_time = val;
                    break;
                case element::cfd_trigger_source_bit:
                    evt.cfd_trigger_source = val;
                    break;
                case element::channel_number:
                    evt.channel_number = val;
                    break;
                case element::crate_id:
                    evt.crate_id = val;
                    break;
                case element::energy:
                    evt.energy = val;
                    break;
                case element::event_length:
                    evt.event_length = val;
                    break;
                case element::event_time_high:
                    evt.event_time_high = val;
                    break;
                case element::event_time_low:
                    evt.event_time_low = val;
                    break;
                case element::finish_code:
                    evt.finish_code = val != 0;
                    break;
                case element::header_length:
                    evt.header_length = val;
                    break;
                case element::slot_id:
                    evt.slot_id = val;
                    break;
                case element::trace_length:
                    evt.trace_length = val;
                    break;
                case element::trace_out_of_range_flag:
                    evt.trace_out_of_range = val != 0;
                    break;
                default:
                    throw error(error::code::invalid_element, "Unknown data element encountered.");
            }
        }

        if (len < evt.event_length) {
            throw error(error::code::invalid_buffer_length,
                        "buffer length is smaller than the reported event.");
        }

        if (evt.event_length != evt.header_length + evt.trace_length / 2) {
            throw error(error::code::invalid_event_length,
                        "Event length does not match header length plus 0.5 * trace_length.");
        }

        unsigned int ets_offset = 0;
        unsigned int esums_offset = 0;
        unsigned int qdc_offset = 0;
        bool has_ets = false;
        bool has_qdc = false;
        bool has_esums = false;
        switch (evt.header_length) {
            case header_length::header:
                break;
            case header_length::header_ets:
                ets_offset = evt.header_length - num_ext_ts_words;
                has_ets = true;
                break;
            case header_length::header_qdc:
                has_qdc = true;
                qdc_offset = evt.header_length - num_qdc_words;
                break;
            case header_length::header_esum:
                has_esums = true;
                esums_offset = evt.header_length - num_esum_words;
                break;
            case header_length::header_esum_ets:
                has_ets = has_esums = true;
                esums_offset = evt.header_length - num_esum_words - num_ext_ts_words;
                break;
            case header_length::header_esum_qdc:
                has_esums = has_qdc = true;
                esums_offset = evt.header_length - num_esum_words - num_qdc_words;
                qdc_offset = evt.header_length - num_qdc_words;
                break;
            case header_length::header_esum_qdc_ets:
                has_esums = has_ets = has_qdc = true;
                esums_offset =
                    evt.header_length - num_ext_ts_words - num_qdc_words - num_esum_words;
                qdc_offset = evt.header_length - num_ext_ts_words - num_qdc_words;
                break;
            case header_length::header_qdc_ets:
                has_qdc = has_ets = true;
                qdc_offset = evt.header_length - num_ext_ts_words - num_qdc_words;
                break;
            default:
                throw error(error::code::invalid_header_length,
                            "unknown header length: " + std::to_string(evt.header_length));
        }

        if (has_ets) {
            evt.external_time_low = data[ets_offset];
            evt.external_time_high = data[ets_offset + 1];
        }

        if (has_esums) {
            for (unsigned int i = 0; i < num_esum_words; i++) {
                evt.energy_sums.push_back(data[esums_offset + i]);
                if (i == num_esum_words - 1) {
                    evt.filter_baseline = util::ieee_float(data[esums_offset + num_esum_words - 1]);
                }
            }
        }

        if (has_qdc) {
            for (unsigned int i = 0; i < num_qdc_words; i++) {
                evt.qdc.push_back(data[qdc_offset + i]);
            }
        }

        if (evt.trace_length > 0) {
            for (size_t w = evt.header_length; w < evt.event_length; ++w) {
                evt.trace.push_back(data[w] & 0xFFFF);
                evt.trace.push_back((data[w] >> 16) & 0xFFFF);
            }
        }

        data += evt.event_length;
        evts.push_back(evt);
    }
    return evts;
}

}  // namespace list_mode
}  // namespace data
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::data::list_mode::event& event) {
    event.output(out);
    return out;
}