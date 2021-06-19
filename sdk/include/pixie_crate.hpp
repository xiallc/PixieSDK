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
#include <pixie_error.hpp>
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
    typedef pixie::error::error error;

    /*
     * Crate
     *
     * A crate is a series of slots that contain modules.
     */
    struct crate
    {
    private:
        /*
         * Module lock
         */
        using lock_type = std::recursive_mutex;
        using lock_guard = std::lock_guard<lock_type>;

    public:
        /*
         * Module lock guard
         */
        class guard {
            lock_type& lock_;
            lock_guard guard_;
        public:
            guard(crate& crate_);
            ~guard() = default;
        };

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
         * Crate revision
         */
        int revision;

        /*
         * A crate contains a number of modules in slots. These modules are
         * online and ready for use.
         */
        module::modules modules;

        /*
         * The offline modules have been detected by are not online. This lets
         * a user check a crate and determine the state of all detected
         * modules.
         */
        module::modules offline;

        /*
         * Firmware for the crate. Check the modules for the ones they have
         * loaded.
         */
        firmware::crate firmware;

        WINDOWS_DLLEXPORT crate();
        WINDOWS_DLLEXPORT virtual ~crate();

        /*
         * Check the crate has been intialised and ready for use. Throws an
         * error if not ready.
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
         * Range checking operator to index modules based on various index
         * types.
         */
        template<typename T> module::module& operator[](T number) {
            size_t number_ = static_cast<size_t>(number);
            if (number_ >= num_modules) {
                throw error(pixie::error::code::module_number_invalid,
                            "module number out of range: " +
                            std::to_string(number));
            }
            return *(modules[number_]);
        }

        /*
         * Return a module indexed by a slot.
         */
        template<typename T> module::module_ptr find(T slot) {
            int slot_ = static_cast<int>(slot);
            auto mod = std::find_if(modules.begin(),
                                    modules.end(),
                                    [slot_](const module::module_ptr m) {
                                        return m->slot == slot_;
                                    });
            if (mod == modules.end()) {
                mod = std::find_if(offline.begin(),
                                   offline.end(),
                                   [slot_](const module::module_ptr m) {
                                       return m->slot == slot_;
                                   });
                if (mod == offline.end()) {
                    throw error(pixie::error::code::module_number_invalid,
                                "module slot not found");
                }
            }
            return *mod;
        }

        /*
         * Initialise the crate and get it ready.
         */
        WINDOWS_DLLEXPORT void initialize(bool reg_trace = false);

        /*
         * Mark a module as offline and move to the offline module list. This
         * invalidates any iterators you may hold to the modules and offline
         * containers.
         */
        void set_offline(module::module_ptr module);

        /*
         * Probe the modules.
         */
        void probe();

        /*
         * Boot all modules.
         */
        WINDOWS_DLLEXPORT void boot();

        /*
         * Assign numbers to the modules by slot. Modules not in the map are
         * forced offline. You can optionally stop this happening but the
         * number for the modules not in the map will be invalid.
         */
        void assign(const module::number_slots& numbers,
                    bool force_offline = true);

        /*
         * Set the firmwares into the modules in the crate.
         */
        WINDOWS_DLLEXPORT void set_firmware();

        /*
         * Import a configuration. Returning a list of loaded modules.
         */
        void import_config(const std::string json_file,
                           module::number_slots& loaded);

        /*
         * Export the configuration
         */
        void export_config(const std::string json_file);

        /*
         * Move offline modules from the online list to offline.
         */
        void move_offlines();

        /*
         * Output the crate details.
         */
        void output(std::ostream& out) const;

    protected:

        virtual void add_module();

    private:
        /*
         * Check the module slots.
         */
        void check_slots();

        /*
         * Check the crate revisions all match.
         */
        void check_revision();

        /*
         * Crate lock
         */
        lock_type lock_;

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

        module::module& operator*() {
            return handle;
        }
        module::module* operator->() {
            return &handle;
        }

    private:
        module::module& handle;
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
