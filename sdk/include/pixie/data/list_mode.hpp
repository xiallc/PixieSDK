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

#include <string>
#include <vector>

namespace xia {
namespace pixie {
namespace data {
namespace list_mode {
/**
 * @brief A data structure for holding a single channel event decoded from a
 *     list-mode data stream.
 *
 * The data present in a list-mode event may vary depending on the
 * firmware being used. This structure represents the totality of all values that
 * may exist within the various firmware. All values stored in this object have
 * been made independent of the specific firmware used to collect them.
 *
 * For example, the `time` has already combined the low and high parts of the
 * 48-bit time stamp with the CFD time if available.
 *
 * We've also attempted to ensure that any special number formats are converted
 * into standard types. The only example of this is the baseline included with
 * the energy sums. The Pixie-16 modules record this value in the IEEE 754
 * number format. We convert this to a double before saving within the data
 * structure.
 */
class event {
public:
    event();
    explicit event(const std::string& json_string);
    ~event();

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
    bool operator==(const event& rhs) const;
    /**
     * @brief Conjugate of the equality operator
     * @param rhs The object that we want to compare
     * @return True if the two objects are not equal.
     */
    bool operator!=(const event& rhs) const;
    /**
     * @brief Uses the 48-bit timestamp to assess whether this event is less
     * than the provided event.
     * @param rhs The object we want to compare
     * @return True if this 48-bit time stamp is less than the one provided.
     */
    bool operator<(const event& rhs) const;
    /**
     * @brief The conjugate of the less than operator
     * @param rhs The object that we're going to compare
     * @return True if the RHS is greater than the
     */
    bool operator>(const event& rhs) const;

    /**
     * @brief True if the CFD was forced to trigger.
     *
     * A forced CFD trigger is needed to avoid that the Pixie-16 channel gets
     * stuck in an infinite loop if the CFD threshold is set too high or no
     * zero crossing is encountered after 32 clock cycles have passed after
     * the local fast trigger point. If the CFD forced trigger bit is 1, the
     * CFD fractional time will be zero.
     */
    bool cfd_forced_trigger{};
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
    bool cfd_trigger_source{};
    /**
     * @brief  If true, then the signal was piled-up with another signal during
     * processing.
     */
    bool finish_code{};
    /**
     * @brief  If true, then the recorded trace was out of range and the energy
     * filter result should not be used.
     */
    bool trace_out_of_range{};

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
    double energy{};
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
    double filter_baseline{};
    /**
     * @brief The arrival time of the event within the system.
     *
     * This value combines the low and high parts of the trigger filter time stamp
     * with the CFD time if available.
     */
    double time{};

    /**
     * @brief  The time at which the CFD crossed the zero-point.
     */
    size_t cfd_time{};
    /**
     * @brief  The channel number that recorded the event.
     *
     * This value starts counting at 0. For example a Pixie-16 Rev F module
     * would range from [0, 15].
     */
    size_t channel_number{};
    /**
     * @brief The crate id for the crate that produced the event.
     * @note The SDK does not support multi-crate systems so this will always be 0.
     */
    size_t crate_id{};
    /**
     * @brief  Upper 16 bits of the time recorded by the trigger filter.
     */
    size_t event_time_high{};
    /**
     * @brief  Lower 32 bits of the time recorded by the trigger filter.
     */
    size_t event_time_low{};
    /**
     * @brief Upper 16 bits of the external time stamp
     */
    size_t external_time_high{};
    /**
     * @brief Lower 32 bits of the external time stamp
     *
     * The Pixie-16 is capable of accepting an external clock signal through
     * one of its front panel connectors and then count such external clock
     * signals before putting those external clock timestamps into the list-mode
     * data event header.
     */
    size_t external_time_low{};
    /**
     * @brief The module's physical slot in the crate this ranges from [2, 14].
     */
    size_t slot_id{};

    /**
     * @brief Energy sums recorded by the module
     *
     * The list mode energy sums (leading, gap and trailing) are the 3 running
     * sums of the digital trapezoidal filter implemented in the Pixie-16 for
     * energy or pulse height measurement.
     */
    std::vector<size_t> energy_sums;
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
    std::vector<size_t> qdc;
    /**
     * @brief  The ADC trace as recorded in the event.
     *
     * The length of the trace can vary depending on the specific settings
     * provided to the DSP by the user.
     */
    std::vector<size_t> trace;

    /**
     * @return The raw 48-bit time stamp excluding any CFD information.
     */
    uint64_t raw_time() const;
    /**
     * @return The 48-bit external time stamp
     */
    uint64_t external_time() const;

    /**
     * @brief Provides streamed output for the class
     * @param out the stream that we'll use for the output
     */
    void output(std::ostream& out) const;

    std::string as_json() const;
};

typedef std::vector<event> events;

}  // namespace list_mode
}  // namespace data
}  // namespace pixie
}  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::data::list_mode::event& event);


#endif  //PIXIESDK_LIST_MODE_HPP
