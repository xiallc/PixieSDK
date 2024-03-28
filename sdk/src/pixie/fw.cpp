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

/** @file fw.cpp
 * @brief Implements data structures and functions to handle firmware.
 */

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>


#include <pixie/error.hpp>
#include <pixie/format.hpp>
#include <pixie/fw.hpp>
#include <pixie/log.hpp>
#include <pixie/utils/io.hpp>
#include <pixie/utils/path.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/utils/time.hpp>

#include <sys/stat.h>

namespace xia {
namespace pixie {
namespace firmware {
/*
 * Param errors
 */
using error = pixie::error::error;

/*
 * Firmware files as filepaths
 */
using files = std::vector<std::string>;

/*
 * Amount of memory being used.
 */
static std::atomic_size_t total_image_size;

/**
 * The system firmware path.
 */
const char* system_firmware_path =
#ifdef XIA_PIXIE_WINDOWS
    "c:/xia/pixie-16/firmware";
#else
    "/usr/local/xia/pixie-16/firmware";
#endif

template<typename T>
T strtonum(const std::string& s, const std::string msg) {
    try {
        return T(std::stoll(s, nullptr, 0));
    } catch (std::invalid_argument& ) {
        throw error(
            error::code::device_image_failure, "firmware: " + msg + ": " + s +
            ": invalid arg");
    } catch (std::out_of_range& ) {
        throw error(
            error::code::device_image_failure, "firmware: " + msg + ": " + s +
            ": out of range");
    }
}

template<typename T>
T strtonum(const format::json::value_type& j, const std::string msg) {
    return strtonum<T>(std::string(j), msg);
}

/**
 * Firmware devices is a map of firmware device types for a set of
 * module revisions.
 */
struct firmware_device {
    bool shared;
    std::vector<int> revisions;
    device_types devices;

    firmware_device() = default;

    template<typename T>
    bool operator==(const T revision) const {
        auto ri = std::find_if(
            std::begin(revisions), std::end(revisions),
            [revision](const auto rev) {
                return revision == rev;
            });
        return ri != std::end(revisions);
    }

    bool operator==(const std::string& device) const {
        auto di = std::find_if(
            std::begin(devices), std::end(devices),
            [device](const auto& dev) {
                return device == dev;
            });
        return di != std::end(devices);
    }
    bool operator!=(const std::string& device) const {
        auto r = *this == device;
        return !r;
    }
};

using firmware_devices = std::vector<firmware_device>;

static const firmware_devices device_map = {
    { false, { 10, 11, 12, 13, 14, 15, 16, 17 }, { "sys", "fippi", "dsp", "var" }}
};

static const firmware_device& firmware_device_find(const int hw_revision) {
    for (const auto& dm : device_map) {
        auto ri = std::find(
            std::begin(dm.revisions), std::end(dm.revisions), hw_revision);
        if (ri != std::end(dm.revisions)) {
            return dm;
        }
    }
    throw error(
        error::code::module_invalid_firmware,
        "firmware: device: no device map for hw revision: " +
            std::to_string(hw_revision));
}

static size_t firmware_device_size(const int hw_revision) {
    for (const auto& dm : device_map) {
        auto ri = std::find(
            std::begin(dm.revisions), std::end(dm.revisions), hw_revision);
        if (ri != std::end(dm.revisions)) {
            return dm.devices.size();
        }
    }
    throw error(
        error::code::module_invalid_firmware,
        "firmware: device: no device map for hw revision: " +
            std::to_string(hw_revision));
}

reader::reader(const image& img_, const size_t default_word_size_)
    : img(img_), default_word_size(default_word_size_), offset(0) {}

image_value_type reader::get(size_t word_size) {
    return read(word_size, true);
}

image_value_type reader::peek(size_t word_size) {
    return read(word_size, false);
}

image_value_type reader::read(size_t word_size, bool inc) {
    if (word_size > sizeof(image_value_type)) {
        std::ostringstream oss;
        oss << "firmware: word-size out of range: word-size=" << word_size;
        throw error(error::code::device_image_failure, oss.str());
    }

    if (word_size == 0) {
        word_size = default_word_size;
    }

    if (offset > (img.size() - word_size)) {
        std::ostringstream oss;
        oss << "firmware: image get out of range: offset=" << offset
            << " word-size=" << word_size
            << " size=" << img.size();
        throw error(error::code::device_image_failure, oss.str());
    }

    image_value_type value = 0;

    for (size_t w = 0; w < word_size; ++w) {
        value = (value << 8) | img[offset + w];
    }

    if (inc) {
        offset += word_size;
    }

    return value;
}

device_detail::device_detail()
    : name("invalid"), mod_revision(0), mod_adc_msps(0), mod_adc_bits(0) {
}

bool device_detail::operator==(const device_detail& other) const {
    return name == other.name && mod_revision == other.mod_revision &&
        mod_adc_msps == other.mod_adc_msps && mod_adc_bits == other.mod_adc_bits;
}

tag_type device_detail::tag() const {
    return pixie::firmware::tag(mod_revision, mod_adc_msps, mod_adc_bits);
}

template<typename T>
void device_detail::output(T& out) const {
    out << "device: name:" << name
        << " mod-rev:" << mod_revision
        << " mod-adc-msps:" << mod_adc_msps
        << " mod-adc-bits:" << mod_adc_bits;
}

firmware::firmware(
    const device_detail& device_, const std::string version_, const mask_type mask_,
    const size_t instance_)
    : tag(device_.tag()), release(not_released), version(version_), mask(mask_),
      instance(instance_), device(device_), crc(0) {}

firmware::firmware(
    const device_detail& device_, const std::string release_, const std::string version_,
    const mask_type mask_, const size_t instance_)
    : tag(device_.tag()), release(release_), version(version_), mask(mask_),
      instance(instance_), device(device_), crc(0) {}

firmware::firmware(
    const device_detail& device_, const release_type& release_, const std::string version_,
    const mask_type mask_, const size_t instance_)
    : tag(device_.tag()), release(release_), version(version_), mask(mask_),
      instance(instance_), device(device_), crc(0) {}

firmware::firmware(const firmware& orig) :
    tag(orig.tag), release(orig.release), version(orig.version), mask(orig.mask),
    instance(orig.instance), device(orig.device), filename(orig.filename), crc(orig.crc) {
}

firmware::firmware(firmware&& from)
    : tag(from.tag), release(from.release), version(from.version), mask(from.mask),
      instance(from.instance), device(from.device), filename(from.filename), crc(from.crc) {
    lock_guard guard(from.lock);
    from.filename.clear();
    from.data.clear();
    from.crc = 0;
}

bool firmware::valid() const {
    auto& dm = firmware_device_find(device.mod_revision);
    auto di = std::find(std::begin(dm.devices), std::end(dm.devices), device.name);
    if (di == std::end(dm.devices)) {
        return false;
    }
    return true;
}

void firmware::check() const {
    auto& dm = firmware_device_find(device.mod_revision);
    auto di = std::find(std::begin(dm.devices), std::end(dm.devices), device.name);
    if (di == std::end(dm.devices)) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: check: invalid device: " + device.name);
    }
}

void firmware::load() {
    lock_guard guard(lock);
    if (data.empty()) {
        util::time::timepoint load_time(true);
        /*
         * Use C and the standard file system interfaces. They are faster
         * than the C++ stream interface
         */
        util::io::file file;
        try {
            file.open(filename, file.flag::ro);
            struct stat sb;
            ::fstat(file.handle, &sb);
            size_t size = size_t(sb.st_size);
            data.resize(size);
            file.read(data, size);
            file.close();
            total_image_size += size;
        } catch (pixie::error::error& e) {
            if (file.valid()) {
                file.close();
            }
            std::stringstream error_tag;
            error_tag << e.what() <<  ": " << tag << ": " << filename;
            throw error(e.type, error_tag.str());
        } catch (...) {
            if (file.valid()) {
                file.close();
            }
            throw;
        }

        util::crc::crc32 crc_;
        crc_.update(data);

        xia_log(log::debug) << "firmware: load: tag=" << tag
                            << " release=" << release.to_string()
                            << std::hex << std::showbase << std::internal
                            << " crc=" << crc_.value
                            << " time=" << load_time
                            << " total=" << total_image_size.load();
    }
}

void firmware::unload() {
    lock_guard guard(lock);
    if (!data.empty()) {
        total_image_size -= data.size();
        data.clear();
        data.shrink_to_fit();
    }
}

size_t firmware::words() {
    lock_guard guard(lock);
    /*
     * Size of the vector should be uint32_t aligned so rounding there
     * should be redundant, but it does not harm so lets keep it.
     */
    return ((size_t(data.size()) - 1) / sizeof(image_value_type)) + 1;
}

void firmware::update_crc(util::crc::crc32& crc_) {
    lock_guard guard(lock);
    if (!data.empty()) {
        crc_.update(data);
    }
}

bool firmware::validate_firmware() {
    util::crc::crc32 cal_crc;
    load();
    update_crc(cal_crc);
    if (cal_crc.value == crc) {
        return true;
    } else {
        unload();
        return false;
    }
}

bool firmware::operator==(const firmware& other) const {
    return
        release == other.release && version == other.version &&
        mask == other.mask && filename == other.filename && device == other.device &&
        (crc == 0 || other.crc == 0 || crc == other.crc);
}

template<typename T>
void firmware::output(T& out) const {
    auto width = out.width();
    {
        util::io::ostream_guard oguard(out);
        out << std::setw(0) << tag
            << ": ver:" << version
            << ": inst:" << instance
            << std::hex << std::showbase << std::internal
            << std::setfill ('0')
            << " mask:" << std::setw (8) << mask
            << ' ';
    }
    device.output(out);
    if (width > 1) {
        out << std::endl << std::setw(width) << ' ';
    }
    out << " size:" << data.size()
        << " file:" << filename;
}

firmware_set::firmware_set(
    const std::string release_, const std::string release_date_)
    : release(release_), release_date(release_date_) {
}

firmware_set::firmware_set(
    const release_type release_, const std::string release_date_)
    : release(release_), release_date(release_date_) {
}

firmware_set::firmware_set() : release(not_released) {
}

firmware_set::firmware_set(const firmware_set& orig)
    : release(orig.release), release_date(orig.release_date),
      firmwares(orig.firmwares), slot(orig.slot) {
}

static bool check_firmware_set(const firmware_set& fw_set, bool valid_check) {
    if (fw_set.firmwares.empty()) {
        if (fw_set.release != not_released) {
            if (!valid_check) {
                throw error(
                    error::code::module_invalid_firmware,
                    "firmware: set: check: release set with no firmware");
            }
        }
        return false;
    }
    auto& tag = fw_set.firmwares[0]->tag;
    auto mod_rev = fw_set.firmwares[0]->device.mod_revision;
    std::vector<std::string> devs;
    for (auto fw : fw_set.firmwares) {
        if (fw->tag != tag) {
            if (!valid_check) {
                throw error(
                    error::code::module_invalid_firmware,
                    "firmware: set: check: tag mismatch: " + tag);
            }
            return false;
        }
        if (fw->device.mod_revision != mod_rev) {
            if (!valid_check) {
                throw error(
                    error::code::module_invalid_firmware,
                    "firmware: set: check: mod revision mismatch: " +
                    std::to_string(mod_rev));
            }
            return false;
        }
        bool found = false;
        for (auto& d : devs) {
            if (fw->device.name == d) {
                found = true;
                break;
            }
        }
        if (!found) {
            devs.push_back(fw->device.name);
        }
    }
    if (valid_check && devs.size() != firmware_device_size(mod_rev)) {
        return false;
    }
    return true;
}

bool firmware_set::empty() const {
    return
        release == not_released && release_date.empty() &&
        firmwares.empty() && slot.empty();
}

bool firmware_set::valid() const {
    return check_firmware_set(*this, true);
}

void firmware_set::check() const {
    check_firmware_set(*this, false);
}

size_t firmware_set::size() const {
    return firmwares.size();
}

bool firmware_set::check_slot(const hw::slot_type slot_) const {
    if (slot.empty() || slot_ == 0) {
        return true;
    }
    return std::find(std::begin(slot), std::end(slot), slot_) != std::end(slot);
}

firmware_set::set_type firmware_set::type() const {
    if (firmwares.empty()) {
        return set_type::undefined;
    }
    auto set_size = firmware_device_size(mod_revision());
    if (device_count() < set_size) {
        return set_type::partial;
    }
    if (firmwares.size() == set_size) {
        if (release == not_released) {
            return set_type::user;
        } else {
            return set_type::release;
        }
    }
    throw error(
        error::code::module_invalid_firmware,
        "firmware: set: type: invalid set size: " + tag());
}

tag_type firmware_set::tag() const {
    if (firmwares.empty()) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: tag: no firmware sets found");
    }
    return firmwares[0]->tag;
}

int firmware_set::mod_revision() const {
    if (firmwares.empty()) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: mod-revision: no firmware sets found");
    }
    return firmwares[0]->device.mod_revision;
}

const device_types& firmware_set::get_devices() const {
    if (firmwares.empty()) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: get-devices: no firmware sets found");
    }
    auto& dm = firmware_device_find(mod_revision());
    return dm.devices;
}

size_t firmware_set::device_count() const {
    device_types devices;
    for (auto fw : firmwares) {
        bool found = false;
        for (auto& name : devices) {
            if (fw->device.name == name) {
                found = true;
                break;
            }
        }
        if (!found) {
            devices.push_back(fw->device.name);
        }
    }
    return devices.size();
}

firmware_ref firmware_set::get(const std::string device) const {
    auto fwi = std::find_if(
        std::begin(firmwares), std::end(firmwares), [&device](const auto& fw) {
            return device == fw->device.name;
        });
    if (fwi == std::end(firmwares)) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: get: device not found: " + device);
    }
    return *fwi;
}

void firmware_set::clear() {
    release = not_released;
    release_date.clear();
    firmwares.clear();
    slot.clear();
}

void firmware_set::add(firmware& fw) {
    fw.check();
    if (!firmwares.empty()) {
        if (fw.tag != tag()) {
            throw error(
                error::code::module_invalid_firmware,
                "firmware: add: invalid tag: module:" + tag() + " fw:" + fw.tag);
        }
        for (auto sfwr : firmwares) {
            auto& sfw = *sfwr;
            if (sfw.device == fw.device) {
                throw error(
                    error::code::module_invalid_firmware,
                    "firmware: add: duplicate firmware: fw:" + fw.tag +
                    " device:" + fw.device.name);
            }
        }
    }
    firmwares.push_back(std::make_shared<firmware>(fw));
}

void firmware_set::merge(const firmware_set& set) {
    if (release != set.release || release_date != set.release_date) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: merge: mismatched releases: module:" + tag() +
            " release:" + set.release.to_string());
    }
    merge_firmwares(set);
    merge_slots(set);
}

void firmware_set::merge_firmwares(const firmware_set& set) {
    /*
     * If there are no firmwares in the set to merge quietly ignore
     * the merge. There is no tag to compare.
     */
    if (set.firmwares.empty()) {
        return;
    }
    check_devices(set);
    if (firmwares.empty()) {
        firmwares = set.firmwares;
    } else {
        auto tag_ = tag();
        for (auto sfw : set.firmwares) {
            /*
             * Check the firmware tag matches?
             */
            if (sfw->device.tag() != tag_) {
                throw error(
                    error::code::module_invalid_firmware,
                    "firmware: set: merge: invalid tag: set-tag:" + tag_ +
                    " fw: device=" + sfw->device.name + " tag=" + sfw->device.tag());
            }
            bool merged = false;
            for (auto& fw : firmwares) {
                /*
                 * An exact firmware match means we accept the
                 * firmware quietly. It could be part of a slot merge.
                 *
                 * Note, this assumes the filenames are absolute paths
                 */
                if (fw->device.name == sfw->device.name) {
                    if (fw->filename != sfw->filename) {
                        throw error(
                            error::code::module_invalid_firmware,
                            "firmware: set: merge: file name changed:" \
                            " device:" + sfw->device.name +
                            " file:" + sfw->filename);
                    }
                    merged = true;
                }
            }
            if (!merged) {
                firmwares.push_back(sfw);
            }
        }
    }
}

void firmware_set::merge_slots(const firmware_set& set) {
    if ((slot.empty() && !set.slot.empty()) ||
        (!slot.empty() && set.slot.empty())) {
        xia_log(log::warning) << "firmware: set: merge: mixed use of all slots and slots";
    }
    if (!set.slot.empty()) {
        slot.insert(slot.end(), set.slot.begin(), set.slot.end());
        auto last = std::unique(slot.begin(), slot.end());
        slot.erase(last, slot.end());
    }
}

void firmware_set::underlay(const firmware_set& set) {
    /*
     * Merge firmware into this set underlaying any existing firmware.
     */
    check_devices(set);
    if (!firmwares.empty()) {
        if (tag() != set.tag()) {
            throw error(
                error::code::module_invalid_firmware,
                "firmware: set: overlay: invalid tag:" + set.tag());
        }
    }
    merge_slots(set);
    for (auto ofw : set.firmwares) {
        bool merged = false;
        for (auto& fw : firmwares) {
            if (fw->device == ofw->device) {
                merged = true;
                break;
            }
        }
        if (!merged) {
            firmwares.push_back(ofw);
        }
    }
    check_devices(*this);
}

void firmware_set::load() {
    for (auto fw : firmwares) {
        fw->load();
    }
}

void firmware_set::unload() {
    for (auto fw : firmwares) {
        fw->unload();
    }
}

firmware_set& firmware_set::operator=(const firmware_set& other) {
    release = other.release;
    release_date = other.release_date;
    firmwares = other.firmwares;
    slot = other.slot;
    return *this;
}

bool firmware_set::operator==(const firmware_set& other) const {
    if (release != other.release ||
        release_date != other.release_date ||
        firmwares.size() != other.firmwares.size()) {
        return false;
    }
    if (!firmwares.empty()) {
        if (tag() != other.tag()) {
            return false;
        }
        for (auto fw : firmwares) {
            bool found = false;
            for (auto ofw : other.firmwares) {
                if (*ofw == *fw) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}

bool firmware_set::operator!=(const firmware_set& other) const {
    return !(*this == other);
}

template<typename T>
void firmware_set::output(T& out) const {
    auto width = out.width();
    out << std::setw(0)
        << "release=" << release.to_string()
        << " slots:";
    if (slot.size() > 0) {
        out << slot.size() << ':';
        const char* delimiter = "";
        for (auto s : slot) {
            out << delimiter << s;
            delimiter = ",";
        }
    } else {
        out << "all";
    }
    size_t count = 1;
    for (auto fw : firmwares) {
        if (width > 1) {
            out << std::endl << std::setw(width) << ' '
                << std::setw(2) << count++ << " - ";
        }
        if (width > 1) {
            out << std::setw(width + 1);
        } else {
            out << ' ';
        }
        out << *fw;
    }
}

tag_type tag(const int hw_revision, const int adc_msps, const int adc_bits) {
    return std::to_string(hw_revision) + '-' + std::to_string(adc_msps) + '-' +
           std::to_string(adc_bits);
}

tag_type tag(const int hw_revision, const hw::configs& configs) {
    /*
     * Return only the unique types from the configurations of the
     * channels.
     */
    std::vector<std::string> tags;
    for (auto& cfg : configs) {
        tags.push_back(tag(hw_revision, cfg.adc_msps, cfg.adc_bits));
    }
    auto last = std::unique(std::begin(tags), std::end(tags));
    tags.erase(last, std::end(tags));
    tag_type t;
    for (auto& ts : tags) {
        if (!t.empty()) {
            t += ',';
        }
        t += ts;
    }
    return t;
}

const char* set_type_label(const firmware_set::set_type type) {
    switch (type) {
    case firmware_set::set_type::undefined:
    default:
        break;
    case firmware_set::set_type::partial:
        return "partial";
    case firmware_set::set_type::user:
        return "user";
    case firmware_set::set_type::release:
        return "release";
    }
    return "undefined";
}

bool valid_devices(const firmware_set& set) {
    if (!set.firmwares.empty()) {
        auto& dm = firmware_device_find(set.mod_revision());
        for (auto fw : set.firmwares) {
            if (dm != fw->device.name) {
                return false;
            }
        }
    }
    return true;
}

void check_devices(const firmware_set& set) {
    if (!valid_devices(set)) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: device: invalid device for tag: module:" + set.tag());
    }
}

bool check(const system& firmwares, const tag_type tag_) {
    return firmwares.find(tag_) != std::end(firmwares);
}

bool check(
    const system& firmwares, const tag_type tag_, const release_type& release,
    const size_t slot) {
    /*
     * A valid check must have a:
     *  - valid release to search for
     *  - tag with loaded firmware
     *  - matching firmware set that is valid
     */
    if (release == not_released) {
        return false;
    }
    auto fwsi = firmwares.find(tag_);
    if (fwsi == std::end(firmwares)) {
        return false;
    }
    const auto& firmware_sets = std::get<1>(*fwsi);
    auto ri = std::find_if(
        std::begin(firmware_sets), std::end(firmware_sets),
        [&release](const auto& fw) {
            return fw->release == release;
        });
    if (ri == std::end(firmware_sets)) {
        return false;
    }
    auto& fw_release = *(*ri);
    if (!fw_release.check_slot(slot)) {
        return false;
    }
    return fw_release.valid();
}

void add(system& firmwares, firmware_set& set) {
    /*
     * A firmware set is partial if it is not complete. Partial sets
     * are not released. A released partial set is invalid and
     * generates an error.
     *
     * A complete unreleased firmware set is a user firmware set.
     *
     * A complete released firmware is a released firmware set.
     *
     * A firmware system is a collection of firmware sets, one for each
     * module tag.
     *
     * A module tag set of firmware must have unique partial, user
     * and released firmware sets. Given partial and user firmware
     * sets are not released there can only be one of them each and
     * there cannot be released firmwares with same release.
     *
     * A partial firmware can be merged into an existing partial
     * firmware set if the result is a partial firmware set or there
     * is no existing user firmware set.
     *
     * A user firmware set can only be added if there is no existing
     * user firmware set. If an user firmware exists and it
     * matches the existing user firmware set the slots are merged
     * under the slot merge rule.
     *
     * A released firmware set can only be added if there is no
     * existing released firmware set of the same release. If a
     * released firmware exists and it matches the existing released
     * firmware set the slots are merged under the slot merge rule.
     *
     * For matching existing firmware sets of the user and released
     * types:
     *
     *  - Slots are merged without regard to the state of the existing
     *    and merging slots. Specifing slots overrides an empty slot
     *    list (which means all slots) so the all status is lost.
     *
     * - The masks must match as masks are not merged. Clashing masks
     *   is considered a firmware configuration management error.
     */
    if (set.empty()) {
        return;
    }
    set.check();
    if (set.release != not_released &&
        set.type() != firmware_set::set_type::release) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: add: released set not complete: module:" + set.tag() +
            " release:" + set.release.to_string());
    }

    auto tag = set.tag();
    auto mi = firmwares.find(tag);
    if (mi == std::end(firmwares)) {
        firmwares[tag] = firmware_set_refs();
    }
    auto& firmware_sets = firmwares[tag];
    /*
     * Not release means partial or user else it is a released
     * firmware set.
     */
    if (set.release == not_released) {
        auto pi = std::find_if(
            std::begin(firmware_sets), std::end(firmware_sets), [](const auto& fw) {
                return fw->type() == firmware_set::set_type::partial;
            });
        auto ui = std::find_if(
            std::begin(firmware_sets), std::end(firmware_sets), [](const auto& fw) {
                return fw->type() == firmware_set::set_type::user;
            });
        auto have_partial = pi != std::end(firmware_sets);
        auto have_user = ui != std::end(firmware_sets);
        auto set_type = set.type();
        switch (set_type) {
        case firmware_set::set_type::partial:
            /*
             * If the provided set is partial and we already have
             * a partial set merge them. If the result is a user
             * set check if we already have a user set. It is an
             * error if there already is one.
             */
            if (have_partial) {
                auto& partial = *(*pi);
                auto new_set = partial;
                new_set.merge(set);
                if (have_user && new_set.type() != firmware_set::set_type::partial) {
                    auto& user = *(*ui);
                    if (new_set != user) {
                        throw error(
                            error::code::module_invalid_firmware,
                            "firmware: set: add: partial set creates user set and a different "
                            "user set already exists: module:" + tag);
                    }
                    user.merge_slots(new_set);
                    firmware_sets.erase(pi);
                    return;
                }
                /*
                 * Replace the partial slot and firmwares.
                 */
                partial.slot = new_set.slot;
                partial.firmwares = new_set.firmwares;
                return;
            }
            break;
        case firmware_set::set_type::user:
            /*
             * If the set is user and we already have a user set
             * see if the sets match. If they match merge the
             * slots else it is an error.
             */
            if (have_user) {
                auto& user = *(*ui);
                if (set != user) {
                    throw error(
                        error::code::module_invalid_firmware,
                        "firmware: set: add: different user set already exists: module:" + tag);
                }
                user.merge_slots(set);
                return;
            }
            break;
        default:
            throw error(
                error::code::internal_failure,
                "firmware: set: add: undefined firmware set: module:" + tag);
        }
    } else {
        /*
         * The set is released. If there is a released set and
         * they are the same verson merge the slots else it is an
         * error.
         */
        auto ri = std::find_if(
            std::begin(firmware_sets), std::end(firmware_sets),
            [&release = set.release](const auto& fw) {
                return release == fw->release;
            });
        auto have_release = ri != std::end(firmware_sets);
        if (have_release) {
            auto& release = *(*ri);
            if (release == set) {
                release.merge_slots(set);
                return;
            }
            throw error(
                error::code::module_invalid_firmware,
                "firmware: set: add: released set already exists: module:" + tag +
                " release:" + set.release.to_string());
        }
    }
    firmwares[tag].push_back(std::make_shared<firmware_set>(set));
}

find_filter::find_filter(const tag_type& tag_, const size_t slot_)
    : tag(tag_), release(not_released), slot(slot_), release_only(false) {
}

find_filter::find_filter(const tag_type& tag_, const release_type& release_, const size_t slot_)
    : tag(tag_), release(release_), slot(slot_), release_only(false) {
}

find_filter::find_filter(const tag_type& tag_, const std::string& release_, const size_t slot_)
    : tag(tag_), release(release_), slot(slot_), release_only(false) {
}

find_filter::find_filter()
    : release(not_released), slot(all_slots), release_only(false) {
}

void find_filter::check() const {
    if (tag.empty()) {
        throw error(
            error::code::module_invalid_firmware, "firmware: find: filter: no tag");
    }
}

void find_filter::set_release(const std::string& release_) {
    release.from_string(release_);
}

static void find(
    firmware_set& firmware, const firmware_set_refs& firmware_sets, const find_filter& filter) {
    if (firmware_sets.empty()) {
        throw error(
            error::code::module_invalid_firmware, "firmware: find: no firmware sets");
    }
    firmware.clear();
    auto pi = std::find_if(
        std::begin(firmware_sets), std::end(firmware_sets), [](const auto& fw) {
            return fw->type() == firmware_set::set_type::partial;
        });
    auto ui = std::find_if(
        std::begin(firmware_sets), std::end(firmware_sets), [](const auto& fw) {
            return fw->type() == firmware_set::set_type::user;
        });
    if (!filter.release_only) {
        if (pi != std::end(firmware_sets)) {
            auto& partial = *(*pi);
            if (partial.check_slot(filter.slot)) {
                firmware.underlay(partial);
            }
        }
        if (ui != std::end(firmware_sets)) {
            auto& user = *(*ui);
            if (user.check_slot(filter.slot)) {
                firmware.underlay(user);
                return;
            }
        }
    }
    if (filter.release != not_released) {
        auto ri = std::find_if(
            std::begin(firmware_sets), std::end(firmware_sets),
            [&release = filter.release](const auto& fw) {
                return fw->release == release;
            });
        if (ri == std::end(firmware_sets)) {
            throw error(
                error::code::module_invalid_firmware,
                "firmware: find: release not found: " + filter.release.to_string());
        }
        auto& fw_release = *(*ri);
        if (fw_release.check_slot(filter.slot)) {
            if (firmware.firmwares.empty()) {
                firmware.release = fw_release.release;
                firmware.release_date = fw_release.release_date;
            }
            firmware.underlay(fw_release);
            return;
        }
    } else {
        firmware_set_ref fw_release;
        for (auto fs : firmware_sets) {
            auto& fw_set = *fs;
            if (fw_set.valid() && fw_set.check_slot(filter.slot)) {
                if (!fw_release || fw_set.release >= fw_release->release) {
                    fw_release = fs;
                }
            }
        }
        if (fw_release) {
            if (firmware.firmwares.empty()) {
                firmware.release = fw_release->release;
                firmware.release_date = fw_release->release_date;
            }
            firmware.underlay(*fw_release);
            return;
        }
    }
    throw error(
        error::code::module_invalid_firmware,
        "firmware: find: firmware set not found: tag:" + filter.tag +
        " release:" + filter.release.to_string() +
        " slot:" + std::to_string(filter.slot));
}

void find(
    firmware_set& firmware, const system& firmware_sets, const find_filter& filter) {
    filter.check();
    auto fwsi = firmware_sets.find(filter.tag);
    if (fwsi == std::end(firmware_sets)) {
        throw error(
            error::code::module_invalid_firmware,
            "firmware: set: find: module type not found: " + filter.tag);
    }
    find(firmware, std::get<1>(*fwsi), filter);
}

void load(system& sys_fw) {
    for (auto& fw_sets : sys_fw) {
        for (auto fw : fw_sets.second) {
            fw->load();
        }
    }
}

void clear(system& sys_fw) {
    for (auto& fw_sets : sys_fw) {
        for (auto fw : fw_sets.second) {
            fw->clear();
        }
    }
}

firmware parse(release_type& release, const std::string fw_desc, const char delimiter) {
    device_detail device;
    std::string filename;
    firmware::crc_type crc = 0;

    release = not_released;

    util::string::strings fields;
    util::string::split(fields, fw_desc, delimiter);

    std::string version;
    firmware::mask_type mask = 0;

    for (auto field : fields) {
        util::string::strings label_value;
        util::string::split(label_value, field, '=');
        if (label_value.size() != 2) {
            throw error(error::code::invalid_value, "invalid firmware field: " + field);
        }
        if (label_value[0] == "release") {
            release.from_string(label_value[1]);
        } else if (label_value[0] == "version") {
            version = label_value[1];
        } else if (label_value[0] == "mask") {
            mask = strtonum<firmware::mask_type>(
                label_value[1], "mask not a number: " + field);
        } else if (label_value[0] == "revision") {
            device.mod_revision = strtonum<int>(
                label_value[1], "module revision not a number: " + field);
        } else if (label_value[0] == "adc-msps") {
            device.mod_adc_msps =
                strtonum<int>(label_value[1], "module ADC MSPS not a number: " + field);
        } else if (label_value[0] == "adc-bits") {
            device.mod_adc_bits =
                strtonum<int>(label_value[1], "module ADC BITS not a number: " + field);
        } else if (label_value[0] == "device") {
            device.name = label_value[1];
        } else if (label_value[0] == "file") {
            filename = label_value[1];
        } else if (label_value[0] == "crc") {
            crc = strtonum<firmware::crc_type>(
                label_value[1], "module CRC not a number: " + field);
        }
    }

    if (version.empty()) {
        throw error(
            error::code::device_image_failure, "firmware version mandatory: " + fw_desc);
    }
    if (mask == 0) {
        throw error(
            error::code::device_image_failure, "firmware mask mandatory: " + fw_desc);
    }
    if (device.mod_revision == 0) {
        throw error(
            error::code::device_image_failure, "firmware revision mandatory: " + fw_desc);
    }
    if (device.mod_adc_msps == 0) {
        throw error(
            error::code::device_image_failure, "firmware ADC MSPS mandatory: " + fw_desc);
    }
    if (device.mod_adc_bits == 0) {
        throw error(
            error::code::device_image_failure, "firmware ADC bits mandatory: " + fw_desc);
    }
    if (device.name.empty()) {
        throw error(
            error::code::device_image_failure, "firmware device mandatory: " + fw_desc);
    }
    if (filename.empty()) {
        throw error(
            error::code::device_image_failure, "firmware file name mandatory: " + fw_desc);
    }

    firmware fw(device, release, version, mask);
    fw.filename = filename;
    fw.crc = crc;

    return fw;
}

void load_firmware_set(system& firmwares, const std::string name, bool no_throw) {
    return load_firmware_set(firmwares, name.c_str(), no_throw);
}

void load_firmware_set(system& firmwares, const char* name, bool no_throw) {
    try {
        xia_log(log::debug) << "firmware: load: parse: " << name;
        std::ifstream input(name, std::ios::in);
        format::json jfs = format::json::parse(input);
        auto module_tags = jfs["product"]["module_tag"];
        auto& jrel = jfs["release"];
        auto& jdevices = jfs["device"];

        /*
         * Firmware can be shared between different module types. It is
         * done this way until the metadata for the firmware is able to
         * handle this.
         */
        for (auto& module_tag : module_tags) {
            util::string::strings tag_fields;
            util::string::split(tag_fields, module_tag, '-');
            if (tag_fields.size() != 3) {
                throw error(error::code::config_json_error,
                            "invalid tag format: " + std::string(name));
            }

            device_detail device;
            device.mod_revision = strtonum<int>(tag_fields[0], "load: revision not a number");
            device.mod_adc_msps = strtonum<int>(tag_fields[1], "load: ADC MSPS not a number");
            device.mod_adc_bits = strtonum<int>(tag_fields[2], "load: ADC BITS not a number");

            std::string release = jrel["version"];
            firmware_set fw_set(release, jrel["date"]);

            auto fw_path = util::path::dirname(name);
            const auto& dm = firmware_device_find(device.mod_revision);
            for (const auto& dev : dm.devices) {
                device.name = dev;
                auto& jdev = jdevices[dev];
                if (jdev.is_array()) {
                    size_t instance = 0;
                    for (auto& jdev_i : jdev.items()) {
                        auto& jdev_i_v = jdev_i.value();
                        std::string version = jdev_i_v["version"];
                        firmware::mask_type mask;
                        if (jdev_i_v.contains("mask")) {
                            mask = strtonum<firmware::mask_type>(jdev_i_v["mask"],
                                                                 "load: mask not a number");
                        } else {
                            mask = 1;
                        }
                        firmware fw(device, release, version, mask, instance++);
                        fw.filename = util::path::join(fw_path, {jdev_i_v["filename"]});
                        fw.crc =
                            strtonum<firmware::crc_type>(jdev_i_v["crc32"], "load: crc not a number");
                        fw_set.add(fw);
                    }
                } else {
                    std::string version = jdev["version"];
                    firmware::mask_type mask;
                    if (jdev.contains("mask")) {
                        mask = jdev["mask"];
                    } else {
                        mask = 1;
                    }
                    firmware fw(device, release, version, mask);
                    fw.filename = util::path::join(fw_path, {jdev["filename"]});
                    fw.crc = strtonum<firmware::crc_type>(jdev["crc32"], "load: crc not a number");
                    fw_set.add(fw);
                }
            }

            xia_log(log::info) << "firmware: load: parse set: release:"
                               << " tag=" << fw_set.tag()
                               << " version=" << fw_set.release.to_string()
                               << " date=" << fw_set.release_date
                               << " firmwares=" << fw_set.firmwares.size();
            if (fw_set.valid()) {
                add(firmwares, fw_set);
            }
        }
    } catch (error& ) {
        throw;
    } catch (format::json::exception& e) {
        if (no_throw) {
            xia_log(log::warning) << "firmware: load: parse firmware file: " << name
                                  << ": " << e.what();
        } else {
            throw error(
                error::code::config_json_error, "parse firmware file: " +
                std::string(e.what()));
        }
    } catch (...) {
        if (no_throw) {
            xia_log(log::warning) << "firmware: load: parse firmware file: " << name
                                  << ": invalid firmware spec file";
        } else {
            throw error(error::code::config_json_error, "invalid firmware spec file");
        }
    }
}

void load_system_firmwares(system& firmwares) {
    auto sys_fw_path_env = std::getenv("PIXIE_SYSTEM_FIRMWARE_PATH");
    if (sys_fw_path_env != nullptr) {
        system_firmware_path = sys_fw_path_env;
    }
    xia_log(log::info) << "firmware: system: loading from "  << system_firmware_path;
    load_firmwares(firmwares, system_firmware_path, true);
}

void load_firmwares(system& firmwares, const std::string path, bool no_throw) {
    load_firmwares(firmwares, path.c_str(), no_throw);
}

void load_firmwares(system& firmwares, const char* path, bool no_throw) {
    files metadata;
    util::path::find_files(path, metadata, ".json");
    for (auto& md : metadata) {
        load_firmware_set(firmwares, md, no_throw);
    }
}

void system_fw_report(std::ostream& out, system& firmwares) {
    format::json jfws = format::json::array();
    for (auto& fws : firmwares) {
        for (auto fw_set : std::get<1>(fws)) {
            format::json fw_set_info;
            fw_set_info["release"] = fw_set->release.to_string();
            fw_set_info["release-data"] = fw_set->release_date;
            fw_set_info["tag"] = fw_set->tag();
            format::json jfw = format::json::array();
            for (auto fw : fw_set->firmwares) {
                format::json fw_info;
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
};  // namespace firmware
};  // namespace pixie
};  // namespace xia

std::ostringstream& operator<<(std::ostringstream& out, const xia::pixie::firmware::firmware& fw) {
    fw.output(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw) {
    fw.output(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware_set& fw_set) {
    fw_set.output(out);
    return out;
}
