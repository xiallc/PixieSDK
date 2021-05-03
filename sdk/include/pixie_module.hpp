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
#include <pixie_channel.hpp>
#include <pixie_fw.hpp>
#include <pixie_hw.hpp>
#include <pixie_log.hpp>
#include <pixie_param.hpp>
#include <pixie_stats.hpp>

#include <hw/i2cm24c64.hpp>
#include <hw/run.hpp>

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
        explicit error(const int num, const int slot,
                       const code type,
                       const std::ostringstream& what);
        explicit error(const int num, const int slot,
                       const code type,
                       const std::string& what);
        explicit error(const int num, const int slot,
                       const code type,
                       const char* what);
        virtual void output(std::ostream& out);
    private:
        std::string make_what(const int num,
                              const int slot,
                              const char* what);
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
         * Revision tags
         */
        enum rev_tag {
            rev_A = 10,
            rev_B,
            rev_C,
            rev_D,
            rev_E,
            rev_F,
            rev_G,
            rev_H
        };

        /*
         * Slot in the crate.
         */
        int slot;

        /*
         * Logical module mapping for this instance of the
         * SDK.
         */
        int number;

        /*
         * Serial number.
         */
        int serial_num;

        /*
         * Revision of the board
         */
        int revision;

        /*
         * Number of channels
         */
        size_t num_channels;

        /*
         * Module's register VM address.
         */
        void* vmaddr;

        /*
         * Channel configs
         */
        hw::configs configs;

        /*
         * EEPROM
         */
        hw::i2c::i2cm24c64::contents eeprom;
        int eeprom_format;

        /*
         * Module parameters
         */
        param::module_var_descs module_var_descriptors;
        param::module_variables module_vars;

        /*
         * Channel parameters, a set per channel.
         */
        param::channel_var_descs channel_var_descriptors;
        channel::channels channels;

        /*
         * Parameter configuration.
         */
        param::address_map param_addresses;

        /*
         * Firmware
         */
        firmware::module firmware;

        /*
         * Diagnostics
         */
        bool reg_trace;

        /*
         * Modules are created by the crate.
         */
        module();
        module(module&& m);
        virtual ~module();
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
        virtual void open(size_t device_number);
        virtual void close();

        /*
         * Range check the channel number.
         */
        template<typename T> void check_channel_num(T number);

        /*
         * Probe the board to see what is running.
         */
        virtual void probe();

        /*
         * Boot the module. If successful it will be online.
         */
        virtual void boot(bool boot_comms = true,
                          bool boot_fippi = true,
                          bool boot_dsp = true);

        /*
         * Initialise the module ready for use.
         */
        virtual void initialize();

        /*
         * Configure the settings from the revision and serial number.
         */
        virtual void config_settings();

        /*
         * Add or get the firmware.
         */
        void add(firmware::module& fw);
        firmware::firmware_ref get(const std::string device);

        /*
         * Read a parameter.
         */
        param::value_type read(const std::string& par);
        param::value_type read(param::module_param par);
        double read(const std::string& par, size_t channel);
        double read(param::channel_param par, size_t channel);

        /*
         * Write a parameter.
         */
        bool write(const std::string& var, param::value_type value);
        bool write(param::module_param var, param::value_type value);
        void write(const std::string& var, size_t channel, double value);
        void write(param::channel_param par, size_t channel, double value);

        /*
         * Read a variable.
         *
         * Note, the variable string version is a convenience function
         * for test tools only. The channel is ignored if the string is
         * for a module variable.
         *
         * `io` true reads the value from the DSP, false returns the
         * module's copy.
         */
        param::value_type read_var(const std::string& var,
                                   size_t channel,
                                   size_t offset = 0,
                                   bool io = true);
        param::value_type read_var(param::module_var var,
                                   size_t offset = 0,
                                   bool io = true);
        param::value_type read_var(param::channel_var,
                                   size_t channel,
                                   size_t offset = 0,
                                   bool io = true);

        /*
         * Write a variable.
         *
         * Note, the variable string version is a convenience function
         * for test tools only. The channel is ignored if the string is
         * for a module variable.
         */
        void write_var(const std::string& var,
                       param::value_type value,
                       size_t channel,
                       size_t offset = 0);
        void write_var(param::module_var var,
                       param::value_type value,
                       size_t offset = 0);
        void write_var(param::channel_var,
                       param::value_type value,
                       size_t channel,
                       size_t offset = 0);

        /*
         * Run control and status
         */
        void run_end();
        bool run_active();

        /*
         * Control tasks
         */
        void aquire_baselines();
        void adjust_offsets();
        void get_traces();
        void set_dacs();

        /*
         * Run tasks
         */
        void start_histograms(hw::run::run_mode mode);
        void start_listmode(hw::run::run_mode mode);

        /*
         * ADC trace
         */
        void read_adc(size_t channel,
                      hw::adc_word* buffer,
                      size_t size,
                      bool run = true);
        void read_adc(size_t channel,
                      hw::adc_trace& buffer,
                      bool run = true);

        /*
         * Find the baseline cut for the range of channels.
         */
        void bl_find_cut(channel::range& channels, param::values& cuts);
        void bl_get(channel::range& channels_,
                    channel::baseline::channels_values& values);

        /*
         * Read the stats
         */
        void read_stats(stats::stats& stats);

        /*
         * Output the module details.
         */
        void output(std::ostream& out) const;
        char revision_label() const;

        /*
         * Read a word.
         */
        hw::word read_word(int reg);

        /*
         * Write a word
         */
        void write_word(int reg, const hw::word value);

        /*
         * DMA block read.
         */
        virtual void dma_read(const hw::address source, hw::words& values);
        virtual void dma_read(const hw::address source,
                              hw::word_ptr values,
                              size_t length);

        /*
         * Locks
         */
        void lock() {
            lock_.lock();
        }

        void unlock() {
            lock_.unlock();
        }

        /*
         * Revision tag operators to make comparisions of a version simpler to
         * code.
         */
        bool operator==(const rev_tag rev) const;
        bool operator!=(const rev_tag rev) const;
        bool operator>=(const rev_tag rev) const;
        bool operator<=(const rev_tag rev) const;
        bool operator<(const rev_tag rev) const;
        bool operator>(const rev_tag rev) const;

    protected:
        /*
         * Initialise the values.
         */
        virtual void erase_values();
        virtual void init_values();

        /*
         * Check the EEPROM for verison 2 format.
         */
        bool eeprom_v2() const;

        /*
         * Module parameter handlers.
         */
        void module_csrb(param::value_type value);
        void slow_filter_range(param::value_type value);
        void fast_filter_range(param::value_type value);

        /*
         * Checks, throws errors.
         */
        void online_check() const;
        void channel_check(const size_t channel) const;

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
        bool dsp_online;

        /*
         * Have hardware?
         */
        bool have_hardware;

        /*
         * PCI bus. The type is opaque.
         */
        bus_handle device;
    };

    inline hw::word
    module::read_word(int reg) {
        hw::word value;
        if (have_hardware) {
            value = hw::read_word(vmaddr, reg);
        } else {
            value = 0;
        }
        if (reg_trace) {
            log(log::debug) << "M r " << std::setfill('0') << std::hex
                            << vmaddr << ':' << std::setw(2) << reg
                            << " => " << std::setw(8) << value;
        }
        return value;
    }

    inline void
    module::write_word(int reg, const hw::word value) {
        if (reg_trace) {
            log(log::debug) << "M w " << std::setfill('0') << std::hex
                            << vmaddr << ':' << std::setw(2) << reg
                            << " <= " << std::setw(8) << value;
        }
        if (have_hardware) {
            hw::write_word(vmaddr, reg, value);
        }
    }

    /*
     * Make a label from the module
     */
    std::string module_label(const module& mod,
                             const char* label = "module");

    /*
     * A list of numbers that can be assigned to modules by slots
     */
    typedef std::pair<int, int> number_slot;
    typedef std::vector<number_slot> number_slots;

    /*
     * A module pointer.
     */
    typedef std::shared_ptr<module> module_ptr;

    /*
     * A container of modules.
     */
    typedef std::vector<module_ptr> modules;

    /*
     * Assign the number to the slots in the rack.
     */
    void assign(modules& mods, const number_slots& numbers);

    /*
     * Sort the modules by index.
     */
    void order_by_number(modules& mods);

    /*
     * Sort the modules by slot.
     */
    void order_by_slot(modules& mods);

    /*
     * Set the module numbers to the slot order.
     */
    void set_number_by_slot(modules& mods);
}
}
}

/*
 * Output stream operator.
 */
std::ostream&
operator<<(std::ostream& out, const xia::pixie::module::module& module);

#endif  // PIXIE_MODULE_H
