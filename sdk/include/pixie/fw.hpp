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

/** @file fw.hpp
 * @brief Defines data structures and functions to handle firmware.
 *
 * The SDK holds all the firmware as @ref firmware objects. Firmware
 * objects are grouped into firmware sets which define the firmware
 * loaded by a module. Firmware sets are ranked by their
 * @firmware_rank. The rank is used to determine the firmware loaded
 * when a module is booted.
 *
 * Partial Firmware Set
 *
 * A partial firmware set is a partial set of firmware for a module. A
 * partial set is not complete and can be assgined to specific
 * slots. Partial firmware are ranked higher than custom and released
 * firmware sets.
 *
 * Cuatom Firmware Set
 *
 * A custom firmware set is a complete set of firmware for a module. A
 * custom firmware set can be assigned to specific slots. Custom
 * firmwares are ranked higher than released firmware sets.
 *
 * A module contains the firmware sets it is booted with.
 */

#ifndef PIXIE_FW_H
#define PIXIE_FW_H

#include <map>
#include <memory>
#include <stdexcept>
#include <mutex>
#include <vector>

#include <pixie/os_compat.hpp>
#include <pixie/utils/crc.hpp>
#include <pixie/utils/version.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects all the firmware related information.
 */
namespace firmware {
/**
 * @brief The image. We keep this as bytes because it makes using it simpler.
 */
using image = std::vector<uint8_t>;

/**
 * @brief Image data type.
 *
 * This is independent to the type used to hold the
 * data. They cannot be the same as data alignment issues come into
 * play. This type matches the hardware requirements for loading the data
 * across the bus hardware.
 */
using image_value_type = uint32_t;

/**
 * Releases are sematically managed and denote articfacts that are
 * are externally managed. Semantic versioning provides a systemantic
 * way to compare related versions.
 */
using release_type = util::version::version;

/**
 * A revision not set defaults to @ref not_released.
 */
inline constexpr auto not_released = util::version::not_released;

/**
 * Firmware tag type
 */
using tag_type = std::string;

/**
 * @brief Reader to handle reading image words from the image.
 *
 * We read the firmware image from disk and into local memory. This allows us
 * to use the data in multiple locations and times without having to read it
 * from disk again. We're also able to perform validations against the image.
 */
struct reader {
    const image& img;
    const size_t default_word_size;
    size_t offset;

    reader(const image& img, size_t default_word_size = sizeof(image_value_type));

    /*
     * A word size of 0, the default, results in the default word size
     * being used.
     */
    image_value_type get(size_t word_size = 0);
    image_value_type peek(size_t word_size = 0);

    void reset() {
        offset = 0;
    }

    size_t size() {
        return img.size();
    }

    size_t remaining() {
        return img.size() - offset;
    }

private:
    image_value_type read(size_t word_size, bool inc);
};

/**
 * @brief Data structure holding the firmware image and associated information.
 */
struct device_detail {
    /**
     * @brief The type of device associated with this firmware.
     *
     * | Firmware Type | Device string |
     * |---|---|
     * | System FPGA | sys |
     * | Signal Processing (FIPPI) FPGA | fippi |
     * | DSP | dsp |
     * | DSP Variable File | var |
     */
    std::string name;
    /**
     * @brief The module's revision as an integer. Ex. Rev F = 0xF = 15.
     */
    int mod_revision;
    /**
     * @brief The ADC sampling frequency for the firmware. Ex. 250
     */
    int mod_adc_msps;
    /**
     * @brief The ADC bit-resolution for the firmware. Ex. 14
     */
    int mod_adc_bits;

    /**
     * Construct with empty or invalid settings
     */
    device_detail();

    /**
     * Compare device details
     */
    bool operator==(const device_detail& other) const;

    /**
     * Return the tag for this type of device
     */
    tag_type tag()  const;

    /**
     * @brief Output the firmware details.
     */
    template<typename T>
    void output(T& out) const;
};

/**
 * @brief Data structure holding the firmware image and associated information.
 */
struct firmware {
    /**
     * Type of a device mask, the machine sized integer
     */
    using mask_type = unsigned int;

    /**
     * Type of the CRC checksum
     */
    using crc_type = util::crc::crc32::value_type;

    /*
     * Firmware lock.
     */
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;

    /**
     * @brief The firmware tag is the concatenation of the revision,
     *          ADC sampling frequency and ADC bit resolution.
     */
    const tag_type tag;
    /**
     * The release of the firmware set it is part of.
     */
    const release_type release;
    /**
     * @brief The firmware version, usually found within the firmware filename.
     */
    const std::string version;
    /**
     * @brief The device mask, specific to the device.
     */
    const mask_type mask;
    /**
     * @brief The firmware instance.
     */
    const size_t instance;

    /**
     * @brief The device's detail
     */
    const device_detail device;

    /**
     * @brief The firmware's file in a file system.
     */
    std::string filename;
    /**
     * CRC checksum
     */
    crc_type crc;

    /**
     * @brief The image data is a char buffer.
     *
     * See @ref words for the number of words of data in the image.
     */
    image data;

    /**
     * @brief The firmware's version, module revision (it can be loaded on) and
     *          device are invariant. The @ref firmware::release is `not_released`.
     * @param[in] device_details The firmware device' details
     * @param[in] version Sets the @ref firmware::version
     * @param[in] mask The device mask. It is device specific
     */
    firmware(
        const device_detail& device, const std::string version, const mask_type mask,
        const size_t instance_ = 0);

    /**
     * @brief The firmware's release, version, module revision (it can be loaded on)
     *        and device are invariant.
     * @param[in] device_details The firmware device' details
     * @param[in] release Sets the @ref firmware::release
     * @param[in] version Sets the @ref firmware::version
     * @param[in] mask The device mask. It is device specific
     */
    firmware(
        const device_detail& device, const std::string release, const std::string version,
        const mask_type mask, const size_t instance_ = 0);
    firmware(
        const device_detail& device, const release_type& release, const std::string version,
        const mask_type mask, const size_t instance_ = 0);

    /**
     * @brief Firmware copy constructor
     */
    firmware(const firmware& orig);
    /**
     * @brief Firmware move constructor
     */
    firmware(firmware&& from);

    /**
     * @brief Is the firmware valid?
     */
    bool valid() const;
    /**
     * @brief Check if the firmware is valid. Raise an error if not.
     */
    void check() const;

    /**
     * @brief Load the firmware from its file.
     */
    void load();
    /**
     * @brief Unload the firmware image from this object.
     */
    void unload();
    /**
     * @brief The number of words of size `image_value_type` of data in the buffer.
     */
    size_t words();

    /**
     * @brief Update the CRC with the contents of this firmware if the
     *        firmware has has been loaded. If the firmware has not been
     *        loaded the crc value passed in remains unchanged.
     */
    void update_crc(util::crc::crc32& crc);
    /**
     * @brief Load firmware and check calculated CRC matches provided CRC.
     *        Unload firmware if CRCs do not match.
     */
    bool validate_firmware();

    /**
     * @brief We only compare the device details.
     */
    bool operator==(const firmware& other) const;

    /**
     * @brief Output the firmware details.
     */
    template<typename T>
    void output(T& out) const;

private:

    lock_type lock;
};

/**
 * @brief Firmware reference.
 */
using firmware_ref = std::shared_ptr<firmware>;

/**
 * @brief A set of firmware references.
 */
using firmware_refs = std::vector<firmware_ref>;

/**
 * @brief A set of device types for a irmware set.
 */
using device_types = std::vector<std::string>;

/**
 * @brief A set of firmware that forms a revision.
 */
struct firmware_set {
    /**
     * Firmware set type
     */
    enum struct set_type {
        undefined,  /**< Undefined firmware set */
        partial,    /**< An imcomplete firmware set */
        user,       /**< A complete firmware set without a revision */
        release     /**< A complete firmware set with a revision */
    };

    /**
     * @brief The firmware can be used in a range of slots.
     */
    using slots = std::vector<hw::slot_type>;

    /**
     * @brief Release of this firmware. It can be a valid release or
     * not released.
     */
    release_type release;

    /**
     * @brief Release date.
     */
    std::string release_date;

    /**
     * @brief The set of firmwares for this revision.
     */
    firmware_refs firmwares;

    /**
     * @brief Specific slots this firmware set loads on.
     *
     * Generic firmware that can be loaded into any slots has no
     * specific slots listed.
     */
    slots slot;

    /**
     * @brief Constuct a firmware set with a revision.
     */
    firmware_set(
        const std::string release, const std::string release_date);
    firmware_set(
        const release_type release, const std::string release_date);

    /**
     * @brief Construct a not released Firmware set. It can be partial
     * or user
     */
    firmware_set();

    /**
     * @brief Copy and move constructors
     */
    firmware_set(const firmware_set& orig);

    /**
     * @brief Is the firmware set empty?
     */
    bool empty() const;
    /**
     * @brief Is the firmware set valid? It must be complete.
     */
    bool valid() const;
    /**
     * @brief Check if the firmware set is valid. Raise an error if not.
     */
    void check() const;
    /**
     * @brief Number of firmwares held in the set
     */
    size_t size() const;

    /**
     * @brief Check if the firmware is for the slot
     */
    bool check_slot(const hw::slot_type slot) const;

    /**
     * @brief Get the type of thisfirmware set
     */
    set_type type() const;
    /**
     * @brief Get the tag for the firmware set
     */
    tag_type tag() const;
    /**
     * @brief Get the module version
     */
    int mod_revision() const;
    /**
     * @brief Get the devices of firmwares a set needs
     * to be user or released
     */
    const device_types& get_devices() const;

    /**
     * Get a count of the of firmware devices in the set
     */
    size_t device_count() const;

    /**
     * @brief Get a firmware from the set fir the requested device
     */
    firmware_ref get(const std::string device) const;

    /**
     * @brief Clear a firmware set.
     */
    void clear();

    /**
     * @brief Add a firmware
     */
    void add(firmware& fw);

    /**
     * @brief Merge a firmware set into this set.
     */
    void merge(const firmware_set& set);

    /**
     * @brief Merge a firmware set's firmwares into this set.
     */
    void merge_firmwares(const firmware_set& set);

    /**
     * @brief Merge a firmware set's slots into this set.
     */
    void merge_slots(const firmware_set& set);

    /**
     * @brief Underlay the firmware set onto this set. The firmware
     *        in this set are not replaced only ones not present are
     *        merged.
     */
    void underlay(const firmware_set& set);

    /**
     * @brief Load the firmwares
     */
    void load();

    /**
     * @brief Unload the firmwares
     */
    void unload();

    /**
     *  @brief Comparision operators
     */
    bool operator==(const firmware_set& other) const;
    bool operator!=(const firmware_set& other) const;

    /**
     *  @brief Copy operator
     */
    firmware_set&  operator=(const firmware_set& other);

    /**
     * @brief Output the firmware details.
     */
    template<typename T>
    void output(T& out) const;
};

/**
 * @brief Firmware set reference.
 */
using firmware_set_ref = std::shared_ptr<firmware_set>;

/**
 * @brief Firmware set references.
 */
using firmware_set_refs = std::vector<firmware_set_ref>;

/**
 * @brief Firmware based on the module revision, ADC MSPS and ADC bits. Firmware
 * is managed independently to the crates and modules.
 */
using system = std::map<std::string, firmware_set_refs>;

/**
 * @brief Make a firmware tag from the input variables.
 * @param hw_revision A module's hardware revision. Ex. Rev F = 15
 * @param adc_msps An ADC sampling frequency. Ex. 250
 * @param adc_bits An ADC bit resolution. Ex. 14
 * @return The firmware's tag made up of the input arguments. Ex. 15-250-14
 */
tag_type tag(const int hw_revision, const int adc_msps, const int adc_bits);

/**
 * @brief Make a firmware tag from the hardware configurations.
 * @param hw_revision A module's hardware revision. Ex. Rev F = 15
 * @param configs List of hardware configurations
 * @return The firmware's tag made up of the input arguments. Ex. 15-250-14
 */
tag_type tag(const int hw_revision, const hw::configs& configs);

/**
 * @brief Return the set type as a label
 * @param type The set type
 * @return The label string
 */
const char* set_type_label(const firmware_set::set_type type);

/**
 * @brief Check the firmwares in a firmware set have valid devices.
 * @param set The firmware set to check
 * @return True if valid else false
 */
bool valid_devices(const firmware_set& set);

/**
 * @brief Check the firmwares in a firmware set have valid devices.
 * @param set The firmware set to check
 */
void check_devices(const firmware_set& set);

/**
 * @brief Check if a firmware set is already in the system.
 * @param firmwares The firmwares in the system
 * @param fw_set The firmware set to check for
 * @return True if we found the firmware in the system.
 */
bool check(const system& firmwares, const firmware_set& fw_set);

/**
 * @brief Check if a firmware tag is in the system.
 * @param firmwares The firmwares in the system
 * @param tag The tag to check
 * @return True if we found firmware for the tag in the system
 */
bool check(const system& firmwares, const tag_type tag);

/**
 * @brief Check if a firmware release is in the system.
 * @param firmwares The firmwares in the system
 * @param tag The tag to check for
 * @param release The release to check for
 * @param slot The slot of the module
 * @return True if we found the firmware release in the system
 */
bool check(
    const system& firmwares, const tag_type tag_, const release_type& release,
    const size_t slot);

/**
 * @brief Add the firmware set to a system.
 * @param firmwares The firmwares already associated with the system.
 * @param fw The firmware object to add to the system.
 */
void add(system& firmwares, firmware_set& set);

/**
 * @brief Find filter to controls the firmware search,
 */
struct find_filter {
    /**
     * Set the slot to all_slots to find firmware for all slots
     */
    static constexpr size_t all_slots = 0;

    tag_type tag;
    release_type release;
    size_t slot;
    bool release_only;

    find_filter(const tag_type& tag, const size_t slot = all_slots);
    find_filter(
        const tag_type& tag, const release_type& release, const size_t slot = all_slots);
    find_filter(
        const tag_type& tag, const std::string& release, const size_t slot = all_slots);
    find_filter();

    void check() const;

    /*
     * Set the release from a string.
     */
    void set_release(const std::string& release);
};

/**
 * @brief Find the matching firmware.
 *
 * If a firmware has a specific slot it is selected over firmware that have no
 * specified slots. Firmwares with no assigned slots are considered defaults.
 *
 * An error is raise if the firmware release cannot be found. If the release is
 * not specified
 *
 * If partial or user firmware sets are present they are returned.
 *
 * @param[out] firmware The found firmware
 * @param[in] firmware_sets The firmware sets to search
 * @param[in] filter The find options
 */
void find(
    firmware_set& firmware, const system& firmware_sets, const find_filter& filter);

/**
 * @brief Load firmware from disk and into a system.
 * @param[in] fw The firmware that we want to load
 */
void load(system& fw);
/**
 * @brief Clear firmware from a system
 * @param[in] fw The firmware that we want to clear
 */
void clear(system& fw);

/**
 * @brief Load firmware set from disk.
 * @param[in] fw The firmware that we want to load
 */
void load(firmware_set& fw_set);
/**
 * @brief Clear firmware from a firmware set
 * @param[in] fw The firmware that we want to clear
 */
void clear(firmware_set& fw);

/**
 * @brief Parse a firmware string.
 *
 * This function is primarily used to parse firmware definition strings in
 * configuration files. The general format is (using the default delimiter)
 *
 * ```
 * rel:ver:rev:adc-msps:adc-bits:device:filename:crc
 * ```
 * ### Examples
 * * `release=1.2.3, version=33339, revision=15, adc-msps=250, adc-bits=16, device=sys, file=syspixie16_revfgeneral_adc250mhz_r33339.bin, crc=0x123456`
 * * `33339:15:250:16:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin:0x123456`
 *
 * @param release A release object to load the parsed revision into
 * @param fw_desc A string describing everything necessary to generate the
 *                  firmware object.
 * @param delimiter If the delmiter is a ' ' (space) any white space is
 *                      considered a delimiter. Default = ":".
 * @return The firmware object created with the parsed data.
 */
firmware parse(release_type& release, const std::string fw_desc, const char delimiter = ':');

/**
 * @brief The default path for the location of firmware in the user's filesystem
 */
extern const char* system_firmware_path;

/**
 * @brief Load firmware sets from the file
 *
 * @param name The JSON file to parse and load
 * @param ignore_error If true log the error and do not throw an error.
*/
void load_firmware_set(system& firmwares, const std::string name, bool ignore_error = false);

/**
 * @brief Load firmware sets from the file
 *
 * @param name The JSON file to parse and load
 * @param ignore_error If true log the error and do not throw an error.
*/
void load_firmware_set(system& firmwares, const char* name, bool ignore_error = false);

/**
 * @brief Returns if the system firmware path exists
 */
bool has_system_firmware_path();

/**
 * @brief Load system firmware sets from the system firmware path into
 *        the crate firmwares
 *
 * @param firmwares The system firmware the firmware sets are loaded into
*/
void load_system_firmwares(system& firmwares);

/**
 * @brief Load the firmware sets from the path into the system firmwares
 *
 * @param firmwares The system firmware the firmware sets are loaded into
 * @param path The path firmware sets are search under for
 * @param ignore_error If true ignore any error
*/
void load_firmwares(
    system& firmwares, const std::string path, bool ignore_error = false);

/**
 * @brief Load the firmware sets from the path into the system firmwares
 *
 * @param firmwares The system firmware the firmware sets are loaded into
 * @param path The path firmware sets are search under for
 * @param ignore_error If true ignore any error
*/
void load_firmwares(system& firmwares, const char* path, bool ignore_error = false);

/**
 * @brief Get a report of all the firmware files in the system
 *
 * @param out Where to output the firmware report
 * @param firmwares The firmwares to report
*/
void system_fw_report(std::ostream& out, system& firmwares);
}  // namespace firmware
}  // namespace pixie
}  // namespace xia

/*
 * Output stream operators.
 */
std::ostringstream& operator<<(std::ostringstream& out, const xia::pixie::firmware::firmware& fw);
std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw);
std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware_set& fw_set);

#endif  // PIXIE_FW_H
