#ifndef PIXIE_MODULE_H
#define PIXIE_MODULE_H

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

#include <atomic>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

#include <pixie_error.hpp>
#include <pixie_fw.hpp>
#include <pixie_hw.hpp>
#include <pixie_param.hpp>

#include <hw/dsp.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
    /*
     * Module errors
     */
    struct error
        : public pixie::error::error {
        typedef pixie::error::code code;
        const int slot;
        explicit error(const int slot, const code type,
                       const std::ostringstream& what);
        explicit error(const int slot, const code type,
                       const std::string& what);
        explicit error(const int slot, const code type,
                       const char* what);
        virtual void output(std::ostream& out);
    };

    /*
     * PCI bus handle is opaque. No direct access as it is
     * specific to the PCI drivers.
     */
    struct pci_bus_handle;
    typedef std::unique_ptr<pci_bus_handle> bus_handle;
    int pci_bus(const bus_handle& device);
    int pci_slot(const bus_handle& device);

    /*
     * Module
     *
     * A module can only be a single specific instance and it is designed to
     * live in a container of modules in a crate. There are limitations on
     * the type of things you can do with a module object. It contains a
     * unique pointer to the opaque bus handle and there can only ever be
     * one instance of a bus handle. If the handle in a module is initialised
     * the handle will be closed when the module destructs. If an instance of
     * a module could be copied and that instance destructs the handle would
     * close the module's device.
     */
    class module
    {
        /*
         * Module lock.
         */
        typedef std::mutex lock_type;
        typedef std::lock_guard<lock_type> lock_guard;

    public:
        /*
         * A handle holds the module locked. This is for transnational
         * operations.
         */
        class guard {
            lock_guard guard_;
        public:
            guard(module& mod);
            ~guard() = default;
        };

        /*
         * Slot in the crate.
         */
        int slot;

        /*
         * Serial number.
         */
        int serial_num;

        /*
         * Revision of the board
         */
        int revision;

        /*
         * Logical module mapping for this instance of the
         * SDK.
         */
        int index;

        /*
         * Module's register VM address.
         */
        void* vmaddr;

        /*
         * DSP
         */
        hw::dsp::dsp dsp;

        /*
         * Parameter descriptors
         */
        param::module_var_descs module_var_descriptors;
        param::channel_var_descs channel_var_descriptors;

        /*
         * Firmware
         */
        firmware::module firmware;

        /*
         * Set up status
         */
        std::string varsdef;

        /*
         * Diagnostics
         */
        bool reg_trace;

        /*
         * Modules are created by the crate.
         */
        module();
        module(module&& m);
        ~module();
        module& operator=(module&& mod);

        /*
         * If the module present?
         */
        bool present();

        /*
         * Has the module been booted and is online?
         */
        bool online();

        /*
         * Open the module and find the device on the bus.
         */
        void open(size_t device_number);
        void close();

        /*
         * Boot the module. If successful it will be online.
         */
        void boot(bool boot_comms = true,
                  bool boot_fippi = true,
                  bool boot_dsp = true);

        /*
         * Initialise the module ready for use.
         */
        void initialize(const std::string varsdef_);

        /*
         * Set the firmware.
         */
        void set(firmware::module& fw);

        /*
         * Get the firmware
         */
        firmware::firmware_ref get(const std::string device);

        /*
         * Output the module details.
         */
        void output(std::ostream& out) const;

        /*
         * IO read 32 bits value.
         */
        inline uint32_t read_32(int reg) {
            uint32_t value = hw::read_32(vmaddr, reg);
            if (reg_trace) {
                std::ios_base::fmtflags oflags(std::cout.flags());
                std::cout << "M r " << std::setfill('0') << std::hex
                          << vmaddr << ':' << std::setw(2) << reg
                          << " => " << std::setw(8) << value
                          << std::endl;
                std::cout.flags(oflags);
            }
            return value;
        }

        /*
         * IO write 32 bits value.
         */
        inline void write_32(int reg,
                             const uint32_t value) {
            if (reg_trace) {
                std::ios_base::fmtflags oflags(std::cout.flags());
                std::cout << "M w " << std::setfill('0') << std::hex
                          << vmaddr << ':' << std::setw(2) << reg
                          << " <= " << std::setw(8) << value
                          << std::endl;
                std::cout.flags(oflags);
            }
            hw::write_32(vmaddr, reg, value);
        }

        void lock() {
            lock_.lock();
        }

        void unlock() {
            lock_.unlock();
        }

    private:
        /*
         * Lock
         */
        lock_type lock_;

        /*
         * In use counter.
         */
        size_t in_use;

        /*
         * Present in the rack.
         */
        bool present_;

        /*
         * Online and ready to use.
         */
        bool online_;

        /*
         * System, FIPPI and DSP online.
         */
        bool comms_fpga;
        bool fippi_fpga;

        /*
         * PCI bus. The type is opaque.
         */
        bus_handle device;
    };

    /*
     * A list of indexes that can be assigned to modules by slots
     */
    typedef std::pair<int, int> index_slot;
    typedef std::vector<index_slot> index_slots;

    /*
     * A container of modules.
     */
    typedef std::vector<module> modules;

    /*
     * Assign the index to the slots in the rack.
     */
    void assign(modules& mods, const index_slots& indexes);

    /*
     * Sort the modules by index.
     */
    void order_by_index(modules& mods);

    /*
     * Sort the modules by slot.
     */
    void order_by_slot(modules& mods);

    /*
     * Set the module index to the slot order.
     */
    void set_index_by_slot(modules& mods);
}
}
}

/*
 * Output stream operator.
 */
std::ostream&
operator<<(std::ostream& out, const xia::pixie::module::module& module);

#endif  // PIXIE_MODULE_H
