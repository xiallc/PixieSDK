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

/** @file list_mode.hpp
 * @brief Defines classes and functions useful for list-mode data processing.
 */

#ifndef PIXIESDK_LIST_MODE_HPP
#define PIXIESDK_LIST_MODE_HPP

#include <chrono>
#include <string>
#include <vector>

#include <pixie/error.hpp>

namespace xia {
namespace pixie {
namespace data {
namespace list_mode {

/*
 * Local error
 */
using error = pixie::error::error;

/**
 * @brief An enumeration providing all combinations of the list-mode data header.
 * @note The trace is not considered part of the list-mode data header!
 * The header size changes depending on the specific configuration of the
 * channel's ::ChanCSRa variable.
 */
enum header_length {
    /**
     * @brief Basic 4 word header
     */
    header = 4,
    /**
     * @brief 4 word header + external time stamps
     */
    header_ets = 6,
    /**
     * @brief 4 word header + energy sums (3 + baseline)
     */
    header_esum = 8,
    /**
     * @brief 4 word header + energy sums + external time stamps
     */
    header_esum_ets = 10,
    /**
     * @brief 4 word header + QDC sums
     */
    header_qdc = 12,
    /**
     * @brief 4 word header + QDC sums + external time stamps
     */
    header_qdc_ets = 14,
    /**
     * @brief 4 word header + energy sums + QDC sums
     */
    header_esum_qdc = 16,
    /**
     * @brief 4 word header + energy sums + QDC sums + external time stamps
     */
    header_esum_qdc_ets = 18
};

/**
 * @brief A data structure for holding a single channel event decoded from a
 *     list-mode data stream.
 *
 * The data present in a list-mode event may vary depending on the
 * firmware being used. This structure represents the totality of all values that
 * may exist within the firmware revisions. We set all values to appropriate
 * defaults if they are not used. The set values will depend on your specific
 * firmware revision. We recommend only using values that are present within your
 * firmware specification.
 *
 * All values stored in this object have been made independent of the firmware
 * used to collect them. For example, the `time` has already combined the low
 * and high parts of the 48-bit time stamp with the CFD time.
 *
 * We've also converted any special number formats into standard types. The
 * only example of this is the baseline included with the energy sums. The
 * Pixie-16 modules record this value in the IEEE 754 number format. We convert
 * this to a double before saving within the data structure.
 */
struct record {
    /**
     * @brief Defines the type of object used for the energy sums data.
     */
    using energy_sums_type = std::vector<size_t>;
    /**
     * @brief Defines the type of object used for the trace
     */
    using trace_type = std::vector<size_t>;
    /**
     * @brief Defines the type of object used for the QDCs.
     */
    using qdc_type = std::vector<size_t>;
    /**
     * @brief Defines a time type that all the times in the object use.
     */
    using time_type = std::chrono::duration<double>;

    record();
    ~record() = default;

    /**
     * @brief Compares the objects to determine if they're equal.
     *
     * Equal in this case doesn't require that all values within the
     * object match. It's sufficient to check only the following values:
     * crate_id, slot_id, channel_number, event_time_{low,high}, and energy.
     *
     * These values represent the data that will always be present in the
     * list-mode data regardless of the specific hardware or firmware
     * configurations.
     *
     * @param rhs The object that we're going to compare with.
     * @return True if the two objects compare as equal.
     */
    bool operator==(const record& rhs) const;
    /**
     * @brief Conjugate of the equality operator
     * @param rhs The object that we want to compare
     * @return True if the two objects are not equal.
     */
    bool operator!=(const record& rhs) const;
    /**
     * @brief Uses the 48-bit timestamp to assess whether this event is less
     * than the provided event.
     * @param rhs The object we want to compare
     * @return True if this 48-bit time stamp is less than the one provided.
     */
    bool operator<(const record& rhs) const;
    /**
     * @brief The conjugate of the less than operator
     * @param rhs The object that we're going to compare
     * @return True if the RHS is greater than the
     */
    bool operator>(const record& rhs) const;

    /**
     * @brief True if the CFD was forced to trigger.
     *
     * A forced CFD trigger is needed to avoid that the Pixie-16 channel gets
     * stuck in an infinite loop if the CFD threshold is set too high or no
     * zero crossing is encountered after 32 clock cycles have passed after
     * the local fast trigger point. If the CFD forced trigger bit is 1, the
     * CFD fractional time will be zero.
     */
    bool cfd_forced_trigger;
    /**
     * @brief  The time at which the CFD crossed the zero-point.
     * @note Units of seconds
     */
    time_type cfd_fractional_time;
    /**
     * @brief Indicates when the CFD zero crossing occurred with respect to the FPGA.
     *
     * The CFD logic in the 250 (or 500) MHz Pixie-16 looks for the CFD zero
     * crossing point at every 4 (or 2 ns) ADC sample. Therefore, when using
     * the 8 ns (or 10 ns) clock to capture the CFD zero crossing point location,
     * there are two (or five) possibilities of the CFD zero crossing: it could
     * either occur at the first 4 ns of the 8 ns interval (odd cycle) or at
     * the second 4 ns of the 8 ns interval (even cycle) (or in any of the 5
     * samples per cycle). The Pixie-16 outputs such “within-cycle” information
     * using the CFD trigger source bits in the event header. Offline analysis
     * program can then know exactly when the CFD zero crossing occurred and
     * the exact CFD fractional time can be known.
     */
    size_t cfd_trigger_source;
    /**
     * @brief  The channel number that recorded the event.
     *
     * This value starts counting at 0. For example a Pixie-16 Rev F module
     * would range from [0, 15].
     */
    size_t channel_number;
    /**
     * @brief The crate id for the crate that produced the event.
     * @note The SDK does not support multi-crate systems so this will always be 0.
     */
    size_t crate_id;
    /**
     * @brief  The energy as recorded by the on-board energy (slow) filter.
     *
     * The energy reported in list mode data is the result of the pulse height
     * measurement. This 16-bit number is not simply the difference between
     * baseline and maximum sample of the pulse, but rather the result of the
     * energy filter corrected for the exponential decay of the signal.
     *
     * If an event is a piled up event, or if the event’s trace out-of-range
     * flag is set to 1, the event’s energy will be defaulted to 0.
     */
    double energy;
    /**
     * @brief Energy sums recorded by the module
     *
     * The list mode energy sums (leading, gap and trailing) are the 3 running
     * sums of the digital trapezoidal filter implemented in the Pixie-16 for
     * energy or pulse height measurement.
     */
    energy_sums_type energy_sums;
    /**
     * @brief The event length is the header length + trace length.
     */
    size_t event_length;
    /**
     * @brief The external time input to the front panel of the system.
     * @note Units of clock cycles
     *
     * The Pixie-16 is capable of accepting an external clock signal through
     * one of its front panel connectors and then count such external clock
     * signals before putting those external clock timestamps into the list-mode
     * data event header.
     *
     * We do not control the clock frequency, so are unable to convert this time
     * into seconds like other time elements.
     *
     */
    time_type external_time;
    /**
     * @brief Baseline that was recorded with the energy sums
     *
     * The baseline is a measurement of the DC offset of the input signal with
     * corrections to the exponential decay of the pulse. The baseline is used
     * during the pulse height computation for a triggered event. It is not
     * equivalent to the DC offset of the signal in value, but rather it is
     * proportional to the DC offset. If the pulse does not decay exponentially,
     * or it has more than one decay constants, the accuracy of the baseline
     * measurement will be compromised.
     */
    double filter_baseline;
    /**
     * @brief The time of arrival of the signal as measured by the on-board filter.
     * @note This time does not include any CFD information.
     * @note Units of seconds
     */
    time_type filter_time;
    /**
     * @brief  If true, then the signal was piled-up with another signal during
     * processing.
     */
    bool finish_code;
    /**
     * @brief The length of the list-mode data header used for processing the data.
     */
    size_t header_length;
    /**
     * @brief QDCs recorded by the module
     *
     * The Pixie-16 supports recording of 8 QDC sums for a triggered event.
     * Each QDC sum is the summation of ADC sample values in a user defined QDC
     * length window. Figure 3-18 illustrates the placement of the 8 QDC sums
     * in relation to the trace recording. Please note, each QDC length can be
     * freely adjusted through software settings (subject to allowed minimum
     * and maximum value). That implies that the 8 QDC sums can be arranged in
     * such ways that certain pulse shape analysis can be easily achieved.
     */
    qdc_type qdc;
    /**
     * @brief The module's physical slot in the crate this ranges from [2, 14].
     */
    size_t slot_id;
    /**
     * @brief The arrival time of the event within the system in seconds.
     * @note Units of seconds
     *
     * This value combines the filter time with the CFD information if available.
     */
    time_type time;
    /**
     * @brief  The ADC trace as recorded in the event.
     *
     * The length of the trace can vary depending on the specific settings
     * provided to the DSP by the user.
     */
    trace_type trace;
    /**
     * @brief The length of the trace as recorded in the header.
     */
    size_t trace_length;
    /**
     * @brief  If true, then the recorded trace was out of range and the energy
     * filter result should not be used.
     */
    bool trace_out_of_range;

    /**
     * @brief Provides streamed output for the class
     * @param out the stream that we'll use for the output
     */
    void output(std::ostream& out) const;
};

/**
 * @brief A type definition to define what a group of records.
 */
using records = std::vector<record>;

/**
 * @brief Converts a record object into a JSON string.
 * @param[in] rec The record that we want to convert into a string.
 * @return The JSON string representation of the record object.
 */
std::string record_to_json(const record& rec);
/**
 * @brief Converts a JSON string object into a record.
 * @param[in] json_string The JSON string that we want converted to a record.
 * @param[in, out] rec The record object to store the converted information.
 * @throws xia::pixie::error::error if the string could not be converted to a record.
 */
void json_to_record(const std::string& json_string, record& rec);

/**
 * @brief A type definition to define an object that holds the binary data.
 */
using buffer = std::vector<uint32_t>;

/**
 * @brief Decodes a Pixie-16 list-mode data block.
 *
 * This function assumes that the first element of the array corresponds to the
 * first word of a record.
 *
 * Any decoding errors result in hard failures. The function will attempt to fail
 * early and fast because the data blocks do not contain marker information.
 * If we encounter corrupted data, or values don't match with what we expect,
 * then we have no way to recover, and we'll have to consider the whole buffer
 * corrupted.
 *
 * @param data A pointer to the array containing the data read out of the module.
 *  The data block does not have to be an integer number of records.
 * @param len The length of the data array. The Pixie-16 list-mode record is
 *  made up of a minimum of 4 32-bit integers. If this value is less than 4,
 *  then we assume that we have a partial record, and put the entire data array
 *  into the leftovers buffer.
 * @param revision The firmware revision used to collect the data. This is
 *  typically found with the DSP firmware.
 * @param frequency The module's ADC sampling frequency that collected the data.
 * @param recs A vector to hold the decoded records. The size of this vector may
 *  be zero if we were unable to decode any complete records from the provided
 *  information.
 * @param leftovers A vector to hold any remaining words that could not be decoded.
 *  These values should be prepended to the next data buffer before passing it
 *  to this function.
 *  If the decoded event length is greater than the remaining words in the
 *  data buffer, then we fill the leftovers buffer with the remaining data. This
 *  typically happens when you've passed in a partial record, or a data block
 *  that contains a partial record at the end.
 */
void decode_data_block(uint32_t* data, size_t len, size_t revision, size_t frequency, records& recs,
                       buffer& leftovers);

}  // namespace list_mode
}  // namespace data
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::pixie::data::list_mode::record& event);


#endif  //PIXIESDK_LIST_MODE_HPP
