#ifndef PIXIE_FW_H
#define PIXIE_FW_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/

#include <stdexcept>
#include <map>
#include <memory>
#include <vector>

#include <pixie_hw.hpp>
#include <xia_windows_compat.hpp>

namespace xia
{
namespace pixie
{
namespace firmware
{
    /*
     * The image. We keep this as bytes because it makes using it simpler.
     */
    typedef std::vector<uint8_t> image;

    /*
     * Image data type. This is independent to the type used to hold the
     * data. They cannot be the same as data alignment issues come into
     * play. This type matches the hardware requirements for loading the data
     * across the bus hardware.
     */
    typedef uint32_t image_value_type;

    /*
     * Reader to handle reading image words from the image.
     */
    struct reader {
        const image& img;
        const size_t default_word_size;
        size_t offset;

        reader(const image& img,
               size_t default_word_size = sizeof(image_value_type));

        /*
         * A word size of 0, the default, results in the default word size
         * being used.
         */
        image_value_type get(size_t word_size = 0);
        image_value_type peek(size_t word_size = 0);

        void reset () {
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

    /*
     * Firmware image.
     */
    struct firmware
    {
        /*
         * Firmware tag
         */
        const std::string tag;

        /*
         * The firmware can be used in a range of slots.
         */
        typedef std::vector<int> slots;

        /*
         * The firmware's file in a file system.
         */
        std::string filename;

        /*
         * The specification.
         */
        const std::string version;
        const int mod_revision;
        const int mod_adc_msps;
        const int mod_adc_bits;
        const std::string device;

        /*
         * Specific slots this firmware loads on. A generic firmware that can
         * loaded into a number of slots has no specific slots listed.
         */
        slots slot;

        /*
         * The image data is a char buffer. See @ref words for the
         * number of words of data in the image.
         */
        image data;

        /*
         * The firmware's version, module revision (it can be loaded on) and
         * device are invariant.
         */
        WINDOWS_DLLEXPORT firmware(const std::string version,
                                   const int mod_revision,
                                   const int mod_adc_msps,
                                   const int mod_adc_bits,
                                   const std::string device);

        /*
         * Load the firmware from it's file.
         */
        void load();

        /*
         * Clear the firmware image from this object.
         */
        void clear();

        /*
         * The number of words of size `image_value_type` of data in the
         * buffer.
         */
        size_t words() const;

        /*
         * We only compare the version, module revision and device. This
         * defines a firmware. A specific version of firmware for a module
         * revision and device is a unique configuration management data point
         * and there can only be a single instance. The file name is not a
         * configuration managed item and is considered a local site specific
         * setting. This operator provides the scoped comparison.
         */
        bool operator==(const firmware& fw) const;

        /*
         * Output the firmware details.
         */
        template <typename T> void output(T& out) const;
    };

    /*
     * Firmware reference.
     */
    typedef std::shared_ptr<firmware> firmware_ref;

    /*
     * Modules have a collection of firmware references.
     */
    typedef std::vector<firmware_ref> module;

     /*
     * Firmware based on the module revision, ADC MSPS and ADC bits. Firmware
     * is managed indepentenly to the crates and modules.
     */
    typedef std::map<std::string, module> crate;

    /*
     * Make a firmware tagl
     */
    std::string tag(const int revision, const int adc_msps, const int adc_bits);

    /*
     * Add the firmware to a crate.
     */
    WINDOWS_DLLEXPORT void add(crate& firmwares, firmware& fw);

    /*
     * Check is a firmware is already in the crate.
     */
    WINDOWS_DLLEXPORT bool check(const crate& firmwares, const firmware& fw);

    /*
     * Find the matching firmware. If a firmware has a specific slot it is
     * selected over firmware that have no specified slots. Firmwares with no
     * asisgned slots are considered defaults.
     */
    firmware_ref find(module& firmwares,
                      const std::string device,
                      const int slot);

    /*
     * Crate firmware.
     */
    void load(crate& fw);
    void clear(crate& fw);

    /*
     * Module firmware.
     */
    void load(module& fw);
    void clear(module& fw);

    /*
     * Parse a firmware string. You can select a suitable delmiter. If the
     * delmiter is a ' ' (space) any white space is considered a delimiter.
     *
     * Format:  ver:rev:device:filename with a ':' delimiter
     *
     *   ver: string
     *   rev: int
     *   device: string
     *   filename: string
     */
    WINDOWS_DLLEXPORT firmware parse(const std::string fw_desc, const char delimiter = ':');
}
}
}

/*
 * Output stream operators.
 */
std::ostringstream&
operator<<(std::ostringstream& out, const xia::pixie::firmware::firmware& fw);
WINDOWS_DLLEXPORT std::ostream&
operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw);
WINDOWS_DLLEXPORT std::ostream&
operator<<(std::ostream& out, const xia::pixie::firmware::module& mod_fw);

#endif  // PIXIE_FW_H
