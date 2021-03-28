#ifndef PIXIE_CRATE_H
#define PIXIE_CRATE_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
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

#include <xia_windows_compat.hpp>

#include <pixie_fw.hpp>
#include <pixie_module.hpp>
#include <hw/fpga.hpp>

namespace xia
{
namespace pixie
{
namespace crate
{
    /*
     * Crate errors
     */
    class error
        : public std::runtime_error {
    public:
        explicit error(const std::string& what);
        explicit error(const char* what);
    };

    /*
     * Number of slots in a crate.
     */
    static const int slots = 12;

    /*
     * Crate
     *
     * A crate is a series of slots that contain modules.
     */
    struct crate
    {
        /*
         * Number of modules present in the crate.
         */
        size_t num_modules;

        /*
         * A crate contains a number of modules in slots.
         */
        module::modules modules;

        /*
         * Firmware for the crate. Check the modules for the ones they have
         * loaded.
         */
        firmware::crate firmware;

        WINDOWS_DLLEXPORT crate(size_t num_modules = slots);
        WINDOWS_DLLEXPORT ~crate();

        WINDOWS_DLLEXPORT void initialize(bool reg_trace = false);
        WINDOWS_DLLEXPORT void boot();

        WINDOWS_DLLEXPORT void set(firmware::crate& firmwares);

        /*
         * Assign indexes to the modules by slot.
         */
        void assign(const module::index_slots& indexes);

        /*
         * Output the crate details.
         */
        void output(std::ostream& out) const;
    };
}
}
}

/*
 * Output stream operator.
 */
WINDOWS_DLLEXPORT std::ostream&
operator<<(std::ostream& out, const xia::pixie::crate::crate& crate);

#endif  // PIXIE_CRATE_H
