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

#include <atomic>

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
         * Hold an instance while using a module.
         */
        class user {
            crate& crate_;
        public:
            user(crate& crate_);
            ~user();
        };

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

        WINDOWS_DLLEXPORT crate();
        WINDOWS_DLLEXPORT ~crate();

        /*
         * Check the crate has been intialised and ready for use. Throws an
         * error is not ready.
         */
        void ready();

        /*
         * Check if the crate is busy?
         */
        bool busy() const;

        /*
         * How many active users are in the crate when this call is made?
         */
        int users() const;

        /*
         * Range chekcing operators to index modules based on various index
         * types.
         */
        template<typename T> module::module& operator[](T number);

        /*
         * Initialise the crate and get it ready. If the number of slots is 0
         * all slots are probed.
         */
        WINDOWS_DLLEXPORT void initialize(size_t num_modules = 0,
                                          bool reg_trace = false);

        /*
         * Boot all modules.
         */
        WINDOWS_DLLEXPORT void boot();

        /*
         * Assign indexes to the modules by slot.
         */
        void assign(const module::index_slots& indexes);

        /*
         * Set the firmwares into the modules in the crate.
         */
        WINDOWS_DLLEXPORT void set_firmware();

        /*
         * Output the crate details.
         */
        void output(std::ostream& out) const;

    private:
        /*
         * Crate ready.
         */
        std::atomic_bool ready_;

        /*
         * Number of active users in the
         */
        std::atomic_int users_;
    };

    /*
     * Module handle. This allows you to access a module during while operating
     * on it. The crate as a user register and the module is locked while this
     * object exists.
     */
    struct module_handle
    {
        module_handle(crate& crate_, size_t number);
        module_handle(crate& crate_, unsigned short number);
        ~module_handle() = default;

        module::module& handle;

    private:
        crate::user user;
        module::module::guard guard;
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
