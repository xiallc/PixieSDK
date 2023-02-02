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
 */

#ifndef PIXIE_FW_H
#define PIXIE_FW_H

#include <map>
#include <memory>
#include <stdexcept>
#include <mutex>
#include <vector>

#include <pixie/os_compat.hpp>

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
typedef std::vector<uint8_t> image;

/**
 * @brief Image data type.
 *
 * This is independent to the type used to hold the
 * data. They cannot be the same as data alignment issues come into
 * play. This type matches the hardware requirements for loading the data
 * across the bus hardware.
 */
typedef uint32_t image_value_type;

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
struct firmware {
    /*
     * Firmware lock.
     */
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;

    /**
     * @brief The firmware tag is the concatenation of the revision,
     *          ADC sampling frequency and ADC bit resolution.
     */
    const std::string tag;

    /**
     * @brief The firmware can be used in a range of slots.
     */
    typedef std::vector<hw::slot_type> slots;

    /**
     * @brief The firmware's file in a file system.
     */
    std::string filename;
    std::string basename() const;

    /**
     * @brief The firmware version, usually found within the firmware filename.
     */
    const std::string version;
    /**
     * @brief The module's revision as an integer. Ex. Rev F = 0xF = 15.
     */
    const int mod_revision;
    /**
     * @brief The ADC sampling frequency for the firmware. Ex. 250
     */
    const int mod_adc_msps;
    /**
     * @brief The ADC bit-resolution for the firmware. Ex. 14
     */
    const int mod_adc_bits;
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
    const std::string device;

    /**
     * @brief Specific slots this firmware loads on.
     *
     * A generic firmware that can loaded into a number of
     * slots has no specific slots listed.
     */
    slots slot;

    /**
     * @brief The image data is a char buffer.
     *
     * See @ref words for the number of words of data in the image.
     */
    image data;

    /**
     * @brief The firmware's version, module revision (it can be loaded on) and
     *          device are invariant.
     * @param[in] version Sets the @ref firmware::version
     * @param[in] mod_revision Sets the @ref firmware::mod_revision
     * @param[in] mod_adc_msps Sets the @ref firmware::mod_adc_msps
     * @param[in] mod_adc_bits Sets the @ref firmware::mod_adc_bits
     * @param[in] device Sets the @ref firmware::device
     */
    firmware(const std::string version, const int mod_revision, const int mod_adc_msps,
             const int mod_adc_bits, const std::string device);

    /**
     * @brief Firmware copy constructor
     */
    firmware(const firmware& orig);

    /**
     * @brief Firmware move constructor
     */
    firmware(firmware&& from);

    /**
     * @brief Load the firmware from its file.
     */
    void load();

    /**
     * @brief Clear the firmware image from this object.
     */
    void clear();

    /**
     * @brief The number of words of size `image_value_type` of data in the buffer.
     */
    size_t words();

    /**
     * @brief We only compare the version, module revision and device.
     *
     * This defines a firmware. A specific version of firmware for a module
     * revision and device is a unique configuration management data point
     * and there can only be a single instance. The file name is not a
     * configuration managed item and is considered a local site specific
     * setting. This operator provides the scoped comparison.
     */
    bool operator==(const firmware& fw) const;

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
typedef std::shared_ptr<firmware> firmware_ref;

/**
 * @brief Modules have a collection of firmware references.
 */
typedef std::vector<firmware_ref> module;

/**
 * @brief Firmware based on the module revision, ADC MSPS and ADC bits. Firmware
 * is managed independently to the crates and modules.
 */
typedef std::map<std::string, module> crate;

/**
 * @brief Make a firmware tag from the input variables.
 * @param revision A module's hardware revision. Ex. Rev F = 15
 * @param adc_msps An ADC sampling frequency. Ex. 250
 * @param adc_bits An ADC bit resolution. Ex. 14
 * @return The firmware's tag made up of the input arguments. Ex. 15-250-14
 */
std::string tag(const int revision, const int adc_msps, const int adc_bits);

/**
 * @brief Add the firmware to a crate.
 * @param firmwares The vector of firmwares already associated with the crate.
 * @param fw The firmware object to add to the crate.
 */
void add(crate& firmwares, firmware& fw);

/**
 * @brief Check if a firmware is already in the crate.
 * @param firmwares The vector of firmwares in the crate
 * @param fw The firmware to look for
 * @return True if we found the firmware in the crate object.
 */
bool check(const crate& firmwares, const firmware& fw);

/**
 * @brief Find the matching firmware.
 *
 * If a firmware has a specific slot it is selected over firmware that have no
 * specified slots. Firmwares with no assigned slots are considered defaults.
 *
 * @param[in] firmwares The module level firmware references to search
 * @param[in] device The type of firmware device that we're looking for. Ex. sys
 * @param[in] slot The slot that should have the firmware.
 * @return
 */
firmware_ref find(module& firmwares, const std::string device, const int slot);

/**
 * @brief Load firmware from disk and into a crate object.
 * @param[in] fw The firmware that we want to load
 */
void load(crate& fw);
/**
 * @brief Clear firmware from a crate
 * @param[in] fw The firmware that we want to clear
 */
void clear(crate& fw);

/**
 * @brief Load firmware from disk and into a module object.
 * @param[in] fw The firmware that we want to load
 */
void load(module& fw);
/**
 * @brief Clear firmware from a module object
 * @param[in] fw The firmware that we want to clear
 */
void clear(module& fw);

/**
 * @brief Parse a firmware string.
 *
 * This function is primarily used to parse firmware definition strings in
 * configuration files. The general format is (using the default delimiter)
 *
 * ```
 * ver:rev:adc-msps:adc-bits:device:filename
 * ```
 * ### Examples
 * * `version=33339, revision=15, adc-msps=250, adc-bits=16, device=sys, file=syspixie16_revfgeneral_adc250mhz_r33339.bin`
 * * `33339:15:250:16:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin`
 *
 * @param fw_desc A string describing everything necessary to generate the
 *                  firmware object.
 * @param delimiter If the delmiter is a ' ' (space) any white space is
 *                      considered a delimiter. Default = ":".
 * @return The firmware object created with the parsed data.
 */
firmware parse(const std::string fw_desc, const char delimiter = ':');

/**
 * @brief The default path for the location of firmware in the user's filesystem
 */
extern std::string system_firmware_path;

/**
 * @brief Data structure holding the information of a firmware file from the filesystem
 */
struct description {
    std::string filename;
    std::string date;
    int version;
    int mod_revision;
    int mod_adc_msps;
    int mod_adc_bits;
    std::string device;
    std::string crc32; /* Reserved; not used */

    description();
    description(
        const std::string& name, const std::string& dev,
        const std::string& fname);

    std::string spec();
};

using descriptions = std::vector<description>;

/**
 * @brief Rev B, Rev C, and Rev D firmwares are interchangeable, so this
 *        checks for existing firmware against each of those revisions
 * 
 * @param revision The module's revision
 * @param config The module's config
 * @param firmwares The crate's firmware
 * @return The tag of a matching firmware if one exists, else "false"
*/
bool revision_tag_check(std::string& m_tag, int revision, hw::config& config, crate& firmwares);

/**
 * @brief Load firmwares found containing the given basepath onto the crate
 *
 * @param basepath The string to search for in the firmware filepaths.
 *                  Default = system_firmware_path
 * @param firmwares The crate firmware to load the found firmware onto
*/
void load_firmwares(crate& firmwares, std::string basepath = system_firmware_path);

/**
 * @brief Get a report of all the firmware files in the system
 * 
 * @param out Where to output the firmware report
 * @param basepath The filepath to collect the system firmware files from
*/
void system_fw_report(std::ostream& out, std::string basepath);

/**
 * @brief Set/override one of the default firmware files in a module
 * 
 * @param firmwares The module firmware we want to set/override
 * @param filepath The filepath of the new firmware file
 * @param device The type of the device of the firmware file. The filepath will be parsed
 *               to find the device if not provided
*/
bool override_default_fw(module& firmwares, const std::string& filepath, std::string device = "");
}  // namespace firmware
}  // namespace pixie
}  // namespace xia

/*
 * Output stream operators.
 */
std::ostringstream& operator<<(std::ostringstream& out, const xia::pixie::firmware::firmware& fw);
std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw);
std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::module& mod_fw);

#endif  // PIXIE_FW_H
