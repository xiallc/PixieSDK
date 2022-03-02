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

static constexpr size_t min_rev = 17562;
static constexpr size_t min_words = 4;
static constexpr size_t num_qdc_words = 8;
static constexpr size_t num_esum_words = 4;
static constexpr size_t num_ext_ts_words = 2;

using json = nlohmann::json;

/**
 * @brief Describes the elements that need to be decoded out of a list-mode event
 */
enum struct element {
    cfd_forced_trigger_bit,
    cfd_fractional_time,
    cfd_trigger_source_bit,
    channel_number,
    crate_id,
    energy,
    event_length,
    event_time_high,
    event_time_low,
    finish_code,
    header_length,
    slot_id,
    trace_length,
    trace_out_of_range_flag,
    END
};

void json_to_record(const std::string& json_string, record& rec) {
    /*
     * We define a dummy double here because the JSON deseralizer can't get to
     * the chrono type that we defined. Using the dummy also ensures that
     * we don't have undefined behavior when missing an entry.
     */
    double dummy;
    try {
        json val = json::parse(json_string);
        val.at("cfd_forced_trigger").get_to(rec.cfd_forced_trigger);

        val.at("cfd_fractional_time").get_to(dummy);
        rec.cfd_fractional_time = record::time_type(dummy);

        val.at("cfd_trigger_source").get_to(rec.cfd_trigger_source);
        val.at("channel_number").get_to(rec.channel_number);
        val.at("crate_id").get_to(rec.crate_id);
        val.at("energy").get_to(rec.energy);
        val.at("energy_sums").get_to(rec.energy_sums);
        val.at("event_length").get_to(rec.event_length);

        val.at("external_time").get_to(dummy);
        rec.external_time = record::time_type(dummy);

        val.at("filter_baseline").get_to(rec.filter_baseline);

        val.at("filter_time").get_to(dummy);
        rec.filter_time = record::time_type(dummy);

        val.at("finish_code").get_to(rec.finish_code);
        val.at("header_length").get_to(rec.header_length);
        val.at("qdc").get_to(rec.qdc);
        val.at("slot_id").get_to(rec.slot_id);

        val.at("time").get_to(dummy);
        rec.time = record::time_type(dummy);

        val.at("trace_out_of_range").get_to(rec.trace_out_of_range);
        val.at("trace").get_to(rec.trace);
    } catch (json::exception& ex) {
        throw xia::pixie::error::error(xia::pixie::error::code::invalid_value,
                                       "could not convert JSON string to record: " +
                                           std::string(ex.what()));
    }
}

std::string record_to_json(const record& evt) {
    json val = {{"cfd_forced_trigger", evt.cfd_forced_trigger},
                {"cfd_fractional_time", evt.cfd_fractional_time.count()},
                {"cfd_trigger_source", evt.cfd_trigger_source},
                {"channel_number", evt.channel_number},
                {"crate_id", evt.crate_id},
                {"energy", evt.energy},
                {"energy_sums", evt.energy_sums},
                {"event_length", evt.event_length},
                {"external_time", evt.external_time.count()},
                {"filter_baseline", evt.filter_baseline},
                {"filter_time", evt.filter_time.count()},
                {"finish_code", evt.finish_code},
                {"header_length", evt.header_length},
                {"qdc", evt.qdc},
                {"slot_id", evt.slot_id},
                {"time", evt.time.count()},
                {"trace", evt.trace},
                {"trace_out_of_range", evt.trace_out_of_range}};
    return val.dump();
}

record::record()
    : cfd_forced_trigger(false), cfd_fractional_time(0), cfd_trigger_source(0), channel_number(0),
      crate_id(0), energy(0), event_length(0), external_time(0), filter_baseline(0), filter_time(0),
      finish_code(false), header_length(0), slot_id(0), time(0), trace_length(0),
      trace_out_of_range(false) {}

bool record::operator==(const record& rhs) const {
    return crate_id == rhs.crate_id && slot_id == rhs.slot_id &&
           channel_number == rhs.channel_number && time == rhs.time && energy == rhs.energy;
}

bool record::operator!=(const record& rhs) const {
    return !operator==(rhs);
}

bool record::operator<(const record& rhs) const {
    return time < rhs.time;
}

bool record::operator>(const record& rhs) const {
    return !operator<(rhs);
}

void record::output(std::ostream& out) const {
    util::ostream_guard flags(out);
    /*
     * We add the to_string on the time to ensure that the double value is not
     * truncated in the stream. See the "Verify streamed output" test case.
     */
    out << std::boolalpha << "crate: " << crate_id << " slot: " << slot_id
        << " channel: " << channel_number << " time: " << std::to_string(uint64_t(time.count()))
        << " energy: " << energy;
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

using descriptor_list = std::vector<element_desc>;

/*
 * NOTE: We put the event length, header length, and trace length first to ensure that we decode
 * those data first and in that order. We need to perform checks on those data to ensure data
 * validity. Everything else falls in alphabetical order to find them easier.
 */

static const descriptor_list descriptors_17562_100 = {
    {element::event_length, 0x3FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0xFFFF0000, 16, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x40000000, 30, 0}};

static const descriptor_list descriptors_29432_100 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0xFFFF0000, 16, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x00007FFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x00008000, 15, 3}};

static const descriptor_list descriptors_30474_100 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_forced_trigger_bit, 0x80000000, 31, 2},
    {element::cfd_fractional_time, 0x7FFF0000, 16, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x00007FFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x00008000, 15, 3}};

static const descriptor_list descriptors_34688_100 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0x7FFF0000, 16, 3},
    {element::cfd_forced_trigger_bit, 0x80000000, 31, 2},
    {element::cfd_fractional_time, 0x7FFF0000, 16, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x80000000, 31, 3}};

static const descriptor_list descriptors_20466_250 = {
    {element::event_length, 0x3FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0xFFFF0000, 16, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x40000000, 30, 0}};

static const descriptor_list descriptors_27361_250 = {
    {element::event_length, 0x3FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0x7FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0x80000000, 31, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x40000000, 30, 0}};

static const descriptor_list descriptors_29432_250 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0x7FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0x80000000, 31, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x00007FFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x00008000, 15, 3}};

static const descriptor_list descriptors_30474_250 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_forced_trigger_bit, 0x80000000, 31, 2},
    {element::cfd_fractional_time, 0x3FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0x40000000, 30, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x00007FFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x00008000, 15, 3}};

static const descriptor_list descriptors_34688_250 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0x7FFF0000, 16, 3},
    {element::cfd_forced_trigger_bit, 0x80000000, 31, 2},
    {element::cfd_fractional_time, 0x3FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0x40000000, 30, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x80000000, 31, 3}};

static const descriptor_list descriptors_46540_250 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0x7FFF0000, 16, 3},
    {element::cfd_forced_trigger_bit, 0x80000000, 31, 2},
    {element::cfd_fractional_time, 0x3FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0x40000000, 30, 2},
    {element::channel_number, 0x0000003F, 0, 0},
    {element::crate_id, 0x00000C00, 10, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000003C0, 6, 0},
    {element::trace_out_of_range_flag, 0x80000000, 31, 3}};

static const descriptor_list descriptors_29432_500 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0xFFFF0000, 16, 3},
    {element::cfd_fractional_time, 0x1FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0xE0000000, 29, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x00007FFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x00008000, 15, 3}};

static const descriptor_list descriptors_34688_500 = {
    {element::event_length, 0x7FFE0000, 17, 0},
    {element::header_length, 0x0001F000, 12, 0},
    {element::trace_length, 0x7FFF0000, 16, 3},
    {element::cfd_fractional_time, 0x1FFF0000, 16, 2},
    {element::cfd_trigger_source_bit, 0xE0000000, 29, 2},
    {element::channel_number, 0x0000000F, 0, 0},
    {element::crate_id, 0x00000F00, 8, 0},
    {element::energy, 0x0000FFFF, 0, 3},
    {element::event_time_high, 0x0000FFFF, 0, 2},
    {element::event_time_low, 0xFFFFFFFF, 0, 1},
    {element::finish_code, 0x80000000, 31, 0},
    {element::slot_id, 0x000000F0, 4, 0},
    {element::trace_out_of_range_flag, 0x80000000, 31, 3}};

static uint64_t make_u64(const uint32_t high, const uint32_t low) {
    return (uint64_t(high) << 32) | uint64_t(low);
}

static double make_u64_double(const uint32_t high, const uint32_t low) {
    return double(make_u64(high, low));
}

static void make_time(record& rec, const size_t freq, const uint32_t filter_low,
                      const uint32_t filter_high, const double cfd_time) {
    double filter_conv;
    switch (freq) {
        case 250:
            filter_conv = 8e-9;
            rec.cfd_fractional_time =
                record::time_type((cfd_time - static_cast<double>(rec.cfd_trigger_source)) * 4e-9);
            break;
        case 500:
            filter_conv = 10e-9;
            rec.cfd_fractional_time = record::time_type(
                (cfd_time - static_cast<double>(rec.cfd_trigger_source) - 1) * 2e-9);
            break;
        default:
            filter_conv = 10e-9;
            rec.cfd_fractional_time = record::time_type(cfd_time * 10e-9);
            break;
    }
    rec.filter_time = record::time_type(make_u64_double(filter_high, filter_low) * filter_conv);
    rec.time = rec.cfd_fractional_time + rec.filter_time;
}

static const descriptor_list& find_element_set(size_t rev, size_t freq) {
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
                            "minimum supported firmware rev is 20466");
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
                            "minimum supported firmware rev is 29432");
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

static double cfd_multiplier(const size_t rev, const size_t freq) {
    switch (freq) {
        case 100:
            if (rev >= 17562 && rev < 30474) {
                return 65536;
            } else {
                return 32768;
            }
        case 250:
            if (rev >= 20466 && rev < 27361) {
                return 65536;
            } else if (rev >= 27361 && rev < 30474) {
                return 32768;
            } else {
                return 16384;
            }
        case 500:
            return 8192;
        default:
            throw error(error::code::invalid_frequency,
                        "invalid frequency: " + std::to_string(freq));
    }
}

void decode_data_block(uint32_t* data, size_t len, size_t revision, size_t frequency, records& recs,
                       buffer& leftovers) {
    if (data == nullptr) {
        throw error(error::code::invalid_buffer, "buffer pointed to an invalid location");
    }
    if (len < min_words) {
        throw error(error::code::invalid_buffer_length,
                    "minimum data buffer size is " + std::to_string(min_words));
    }
    if (revision < min_rev) {
        throw error(error::code::invalid_revision,
                    "minimum supported firmware rev is " + std::to_string(min_rev));
    }

    recs.clear();
    auto* data_start = data;
    auto* data_end = data_start + len;
    auto& core_elements = find_element_set(revision, frequency);

    while (data < data_end) {
        record evt;
        uint32_t event_time_low;
        uint32_t event_time_high;
        double cfd_fractional_time;
        for (const auto& ele : core_elements) {
            auto val = (data[ele.header_index] & ele.value) >> ele.start_bit;
            switch (ele.type) {
                case element::event_length:
                    if (len < val) {
                        throw error(error::code::invalid_buffer_length,
                                    "buffer length is smaller than the reported event");
                    }
                    evt.event_length = val;
                    break;
                case element::header_length:
                    evt.header_length = val;
                    if (revision < 30980) {
                        switch (val) {
                            case header_length::header_ets:
                            case header_length::header_esum_ets:
                            case header_length::header_qdc_ets:
                            case header_length::header_esum_qdc_ets:
                                throw error(error::code::invalid_header_length,
                                            "external timestamps not introduced until revision 30980");
                            default:
                                break;
                        }
                    }
                    break;
                case element::trace_length:
                    /*
                     * The trace length is stored as the number of samples, so we need to
                     * divide by two to when comparing with the event length, which is
                     * provided as the number of 32-bit words.
                     */
                    if (evt.event_length != evt.header_length + val / 2) {
                        throw error(error::code::invalid_event_length,
                                    "Event length does not match header length plus 0.5 * trace_length");
                    }
                    evt.trace_length = val;
                    break;
                case element::cfd_forced_trigger_bit:
                    evt.cfd_forced_trigger = val != 0;
                    break;
                case element::cfd_fractional_time:
                    cfd_fractional_time =
                        static_cast<double>(val) / cfd_multiplier(revision, frequency);
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
                    evt.energy = static_cast<double>(val);
                    break;
                case element::event_time_high:
                    event_time_high = val;
                    break;
                case element::event_time_low:
                    event_time_low = val;
                    break;
                case element::finish_code:
                    evt.finish_code = val != 0;
                    break;

                case element::slot_id:
                    evt.slot_id = val;
                    break;
                case element::trace_out_of_range_flag:
                    evt.trace_out_of_range = val != 0;
                    break;
                default:
                    throw error(error::code::invalid_element, "Unknown data element encountered");
            }
        }

        /*
         * TODO: I'm leaving this here as a note that we should check, but haven't decided if this should be hard or soft.
         * We could treat this as not an error and just force the CFD time to be zero instead.
         * The downside to that would be it masks issues in the firmware.
         */
        /*
        if (evt.cfd_forced_trigger && evt.cfd_fractional_time != 0) {
            throw error(error::code::invalid_cfd_time,
                        "data corruption: cfd was forced but still recorded a time");
        }
         */
        make_time(evt, frequency, event_time_low, event_time_high);

        unsigned int ets_offset = evt.header_length - num_ext_ts_words;
        unsigned int esums_offset = 0;
        unsigned int qdc_offset = 0;
        bool has_ets = false;
        bool has_qdc = false;
        bool has_esums = false;
        switch (evt.header_length) {
            case header_length::header:
                break;
            case header_length::header_ets:
                has_ets = true;
                break;
            case header_length::header_esum:
                has_esums = true;
                esums_offset = evt.header_length - num_esum_words;
                break;
            case header_length::header_esum_ets:
                has_ets = has_esums = true;
                esums_offset = evt.header_length - num_esum_words - num_ext_ts_words;
                break;
            case header_length::header_qdc:
                has_qdc = true;
                qdc_offset = evt.header_length - num_qdc_words;
                break;
            case header_length::header_qdc_ets:
                has_qdc = has_ets = true;
                qdc_offset = evt.header_length - num_ext_ts_words - num_qdc_words;
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
            default:
                throw error(error::code::invalid_header_length,
                            "unknown header length: " + std::to_string(evt.header_length));
        }

        if (has_ets) {
            evt.external_time =
                record::time_type(make_u64_double(data[ets_offset + 1], data[ets_offset]));
        }

        if (has_esums) {
            for (unsigned int i = 0; i < num_esum_words; i++) {
                if (i != num_esum_words - 1) {
                    evt.energy_sums.push_back(data[esums_offset + i]);
                } else {
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
        recs.push_back(evt);
    }
}

}  // namespace list_mode
}  // namespace data
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::pixie::data::list_mode::record& event) {
    event.output(out);
    return out;
}