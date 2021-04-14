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

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <pixie_module.hpp>
#include <pixie_log.hpp>
#include <pixie_util.hpp>

#include <hw/dsp.hpp>
#include <hw/fpga_comms.hpp>
#include <hw/fpga_fippi.hpp>
#include <hw/pcf8574.hpp>

#include <pixie16sys_defs.h>

#include <PlxApi.h>

namespace xia
{
namespace pixie
{
namespace module
{
    static std::string
    module_label(const int num, const int slot)
    {
        std::ostringstream oss;
        oss << "module [num=" << num << ",slot=" << slot << "]";
        return oss.str();
    }

    error::error(const int num, const int slot,
                 const code type, const std::ostringstream& what)
        : pixie::error::error(type, make_what(num, slot, what.str().c_str()))
    {
    }

    error::error(const int num, const int slot,
                 const code type, const std::string& what)
        : pixie::error::error(type, make_what(num, slot, what.c_str()))
    {
    }

    error::error(const int num, const int slot,
                 const code type, const char* what)
        : pixie::error::error(type, make_what(num, slot, what))
    {
    }

    void
    error::output(std::ostream& out)
    {
        ostream_guard flags(out);
        out << std::setfill(' ')
            << "error: code=" << std::setw(2) << result()
            << ' ' << what();
    }

    std::string
    error::make_what(const int num, const int slot, const char* what_)
    {
        std::ostringstream oss;
        oss << module_label(num, slot) << ": " << what_;
        return oss.str();
    }

    /*
     * PLX PCI vendor and device id
     */
    const int vendor_id = 0x10b5;
    const int device_id = 0x9054;

    struct pci_bus_handle {
        int device_number;
        PLX_DEVICE_OBJECT handle;
        PLX_DEVICE_KEY key;
        pci_bus_handle();
    };

    pci_bus_handle::pci_bus_handle()
        : device_number(-1)
    {
        ::memset(&key, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));
        key.VendorId = vendor_id;
        key.DeviceId = device_id;
    }

    int
    pci_bus(const bus_handle& device)
    {
        if (device) {
            return device->key.bus;
        }
        return -1;
    }

    int
    pci_slot(const bus_handle& device)
    {
        if (device) {
            return device->key.slot;
        }
        return -1;
    }

    /*
     * EEPROM module configuration handles the various format versions.
     */
    struct module_eeprom_format
    {
        std::pair<int, int> revisions;
        int version;
    };

    static const std::vector<module_eeprom_format> module_eeprom_formats =
    {
        { { 0x0a, 0x0f }, 0 },
        { { 0x0f, 0x0f }, 1 },
        { { 0x48, 0xff }, 2 }
    };

    /*
     * The early Pixie module EEPROMs do not contain the ADC data. Provide a
     * table of configuration data.
     */
    struct module_config
    {
        std::pair<int, int> serial_num;
        int adc_bits;
        int adc_msps;
        int num_channels;
        int eeprom_format;
        std::vector<int> revisions;
    };

    static const std::vector<module_config> module_configs =
    {
        { {    0,  255 }, 12, 100, 16, 0, { 0xA } },
        { {  256,  274 }, 12, 100, 16, 1, { 0xB, 0xC, 0xD } },
        { { 1000, 1034 }, 12, 250, 16, 1, { 0xF } }
    };

    module::guard::guard(module& mod)
        : guard_(mod.lock_)
    {
    }

    module::module()
        : slot(0),
          number(-1),
          serial_num(0),
          revision(0),
          adc_bits(0),
          adc_msps(0),
          num_channels(0),
          vmaddr(nullptr),
          dsp(*this),
          eeprom_format(-1),
          module_var_descriptors(param::get_module_var_descriptors()),
          channel_var_descriptors(param::get_channel_var_descriptors()),
          reg_trace(false),
          in_use(0),
          present_(false),
          online_(false),
          comms_fpga(false),
          fippi_fpga(false),
          device(std::make_unique<pci_bus_handle>())
    {
        module_values.resize(module_var_descriptors.size());
    }

    module::module(module&& m)
        : slot(m.slot),
          number(m.number),
          serial_num(m.serial_num),
          revision(m.revision),
          adc_bits(m.adc_bits),
          adc_msps(m.adc_msps),
          num_channels(m.num_channels),
          vmaddr(m.vmaddr),
          dsp(*this),
          eeprom(m.eeprom),
          eeprom_format(m.eeprom_format),
          module_var_descriptors(std::move(m.module_var_descriptors)),
          module_values(std::move(m.module_values)),
          channel_var_descriptors(std::move(m.channel_var_descriptors)),
          channel_values(std::move(m.channel_values)),
          firmware(std::move(m.firmware)),
          reg_trace(m.reg_trace),
          in_use(0),
          present_(m.present_),
          online_(m.online_),
          comms_fpga(m.comms_fpga),
          fippi_fpga(m.fippi_fpga),
          device(std::move(m.device))
    {
        m.slot = 0;
        m.number = -1;
        m.serial_num = 0;
        m.revision = 0;
        m.adc_bits = 0;
        m.adc_msps = 0;
        m.num_channels = 0;
        m.vmaddr = nullptr;
        m.eeprom.clear();
        m.eeprom_format = -1;
        m.module_var_descriptors =
            param::module_var_descs(param::get_module_var_descriptors());
        m.module_values.resize(m.module_var_descriptors.size());
        m.channel_var_descriptors =
            param::channel_var_descs(param::get_channel_var_descriptors());
        m.channel_values.clear();
        m.present_ = false;
        m.online_ = false;
        m.comms_fpga = false;
        m.fippi_fpga = false;
    }

    module::~module()
    {
        /*
         * Catch an error here and log.
         */
        try {
            close();
        } catch(error& e) {
            log(log::error) << e;
        }
        device.release();
    }

    module&
    module::operator=(module&& m)
    {
        lock_guard guard(lock_);
        lock_guard guard_m(m.lock_);

        if (in_use != 0 || m.in_use != 0) {
            throw error(number, slot, error::code::internal_failure,
                        "cannot move modules when in use");
        }

        slot = m.slot;
        number = m.number;
        serial_num = m.serial_num;
        revision = m.revision;
        adc_bits = m.adc_bits;
        adc_msps = m.adc_msps;
        num_channels = m.num_channels;
        vmaddr = m.vmaddr;
        dsp = std::move(m.dsp);
        eeprom = std::move(m.eeprom);
        eeprom_format = m.eeprom_format;
        module_var_descriptors = std::move(m.module_var_descriptors);
        channel_var_descriptors = std::move(m.channel_var_descriptors);
        module_values = std::move(m.module_values);
        channel_values = std::move(m.channel_values);
        reg_trace = m.reg_trace;
        present_ = m.present_;
        online_ = m.online_;
        comms_fpga = m.comms_fpga;
        fippi_fpga = m.fippi_fpga;

        device = std::move(m.device);

        m.slot = 0;
        m.number = -1;
        m.serial_num = 0;
        m.revision = 0;
        m.adc_bits = 0;
        m.adc_msps = 0;
        m.num_channels = 0;
        m.vmaddr = nullptr;
        m.eeprom.clear();
        m.eeprom_format = -1;
        m.reg_trace = false;
        m.present_ = false;
        m.online_ = false;
        m.comms_fpga = false;
        m.fippi_fpga = false;

        return *this;
    }

    bool
    module::present()
    {
        lock_guard guard(lock_);
        return present_;
    }

    bool
    module::online()
    {
        lock_guard guard(lock_);
        return online_;
    }

    void
    module::open(size_t device_number)
    {
        log(log::debug) << "module: open: device-number=" << device_number;

        if (online_) {
            throw error(number, slot,
                        error::code::module_already_open,
                        "module already open");
        }

        if (device->device_number < 0) {
            PLX_STATUS ps;

            ps = ::PlxPci_DeviceFind(&device->key, device_number);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI find: device: " << device_number
                    << " : " << ps;
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            device->device_number = device_number;

            present_ = true;

            ps = ::PlxPci_DeviceOpen(&device->key, &device->handle);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI open: device: " << device_number
                    << " : " << ps;
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            /*
             * For PLX 9054, Space 0 is at PCI BAR 2.
             */
            ps = PlxPci_PciBarMap(&device->handle, 2, (VOID**) &vmaddr);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "PCI BAR map: device: " << device_number
                    << " : " << ps;
                throw error(number, slot,
                            error::code::module_initialize_failure,
                            oss);
            }

            hw::i2c::i2cm24c64 i2cm24c64(*this, I2CM24C64_ADDR,
                                         (1 << 0) | (1 << 3), 1 << 1, 1 << 2);
            i2cm24c64.read(0, 128, eeprom);

            if (eeprom.size() != 128) {
                std::ostringstream oss;
                oss << "eeprom read: device: " << device_number
                    << " : invalid data length:" << eeprom.size();
                throw error(number, slot,
                            error::code::module_info_failure,
                            oss);
            }

            hw::i2c::pcf8574 pio(*this, PCF8574_ADDR,
                                 (1 << 0) | (1 << 3), 1 << 1, 1 << 2);

            slot = (pio.read_a_byte() & 0xf8) >> 3;

            if (!eeprom_v2()) {
                /*
                 *  Starting with serial number 256, serial number is stored in the
                 *  first two bytes of EEPROM, followed by revision number, which
                 *  is at least 11 (i.e. Rev-B)
                 */
                revision = eeprom[2];

                if (revision >= 0xB) {
                    serial_num =
                        (static_cast<int>(eeprom[1]) << 8) |
                        static_cast<int>(eeprom[0]);
                } else {
                    serial_num = static_cast<int>(eeprom[0]);
                }

                if (serial_num == 0xFFFF) {
                    throw error(number, slot,
                                error::code::module_initialize_failure,
                                "invalid serial number: EEPROM erased");
                }

                if (serial_num > 1034) {
                    adc_bits = static_cast<int>(eeprom[99]);
                    adc_msps =
                        (static_cast<int>(eeprom[99 + 1]) << 8) |
                        static_cast<int>(eeprom[99 + 2]);
                } else {
                    for (const auto& config : module_configs) {
                        if (serial_num >= std::get<0>(config.serial_num) &&
                            serial_num <= std::get<1>(config.serial_num)) {
                            adc_bits = config.adc_bits;
                            adc_msps = config.adc_msps;
                            eeprom_format = config.eeprom_format;
                            num_channels = config.num_channels;
                        break;
                        }
                    }
                }

                if (adc_bits == 0) {
                    std::ostringstream oss;
                    oss << "unknown serial number: " << serial_num;
                    throw error(number, slot,
                                error::code::module_initialize_failure, oss.str());
                }
            } else {
                throw error(number, slot, error::code::module_initialize_failure,
                    "eeprom format 2 not supported");
            }
        }
    }

    void
    module::close()
    {
        if (device && device->device_number >= 0) {
            PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
            PLX_STATUS ps_close;

            log(log::debug) << "module: close: device-number=" << device->device_number;

            if (vmaddr != nullptr) {
                ps_unmap_bar = ::PlxPci_PciBarUnmap(&device->handle, &vmaddr);
                vmaddr = nullptr;
            }

            ps_close = ::PlxPci_DeviceClose(&device->handle);

            device->device_number = -1;
            online_ = false;
            present_ = false;

            /*
             * A single error for both operations and the device is always
             * closed..
             */
            if (ps_unmap_bar != PLX_STATUS_OK || ps_close != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "Pixie PCI ";
                if (ps_unmap_bar != PLX_STATUS_OK) {
                    oss << "BAR unmap";
                }
                if (ps_unmap_bar != PLX_STATUS_OK &&
                    ps_close != PLX_STATUS_OK) {
                    oss << " and ";
                }
                if (ps_close != PLX_STATUS_OK) {
                    oss << "close";
                }
                oss << ": device: " << device->device_number
                    << " : " << ps_unmap_bar << ", " << ps_close;
                throw error(number, slot,
                            error::code::module_close_failure,
                            oss.str());
            }
        }
    }

    template<typename T> void
    module::check_channel_num(T num)
    {
        if (num >= num_channels ||
            (std::is_signed<T>::value && num < 0)) {
            throw error(number, slot,
                        error::code::channel_number_invalid,
                        "channel number out of range");
        }
    }

    void
    module::probe()
    {
        hw::fpga::comms comms(*this);
        comms_fpga = comms.done();

        hw::fpga::fippi fippi(*this);
        fippi_fpga = fippi.done();

        dsp.online = dsp.done();

        log(log::info) << std::boolalpha
                       <<  module_label(number, slot)
                       << ": probe: sys=" << comms_fpga
                       << " fippi=" << fippi_fpga
                       << " dsp=" << dsp.online;

        firmware::firmware_ref vars = get("var");
        param::load(vars,
                    module_var_descriptors,
                    channel_var_descriptors);
        module_values.clear();
        module_values.resize(module_var_descriptors.size());
        channel_values.clear();
        channel_values.resize(channel_var_descriptors.size());
    }

    void
    module::boot(bool boot_comms, bool boot_fippi, bool boot_dsp)
    {
        if (online_) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "module is online");
        }

        if (boot_comms) {
            if (comms_fpga) {
                log(log::info) << module_label(number, slot)
                               << ": comms already loaded";
            }
            firmware::firmware_ref fw = get("sys");
            hw::fpga::comms comms(*this);
            comms.boot(fw->data);
            comms_fpga = true;
        }

        if (boot_fippi) {
            if (fippi_fpga) {
                log(log::info) << module_label(number, slot)
                               << ": fippi already loaded";
            }
            firmware::firmware_ref fw = get("fippi");
            hw::fpga::fippi fippi(*this);
            fippi.boot(fw->data);
            fippi_fpga = true;
        }

        if (boot_dsp) {
            if (dsp.online) {
                log(log::info) << module_label(number, slot)
                               << ": dsp already running";
            }
            firmware::firmware_ref fw = get("dsp");
            dsp.boot(fw->data);
        }

        firmware::firmware_ref vars = get("var");
        param::load(vars,
                    module_var_descriptors,
                    channel_var_descriptors);

        log(log::info) << std::boolalpha
                       << "module: boot: sys-fpga=" << comms_fpga
                       << " fippi-fpga=" << boot_fippi
                       << " dsp=" << dsp.online;

        online_ = comms_fpga && fippi_fpga && dsp.online;
    }

    void
    module::initialize()
    {
    }

    void
    module::set(firmware::module& fw)
    {
        if (online_) {
            throw error(number, slot,
                        error::code::module_invalid_operation,
                        "module is online");
        }
        firmware.clear();
        std::copy(fw.begin(), fw.end(),
                  std::back_inserter(firmware));
    }

    firmware::firmware_ref
    module::get(const std::string device)
    {
        lock_guard guard(lock_);
        /*
         * First check if a slot assigned firmware exists for this
         * device. If not see if a default is available.
         */
        for (auto& fwr : firmware) {
            if (fwr->device == device) {
                return fwr;
            }
        }
        std::ostringstream oss;
        oss << "firmware not found: slot=" << slot
            << ": device=" << device
            << " firmwares=" << firmware.size();
        throw error(number, slot,
                    error::code::module_invalid_firmware,
                    oss.str());
    }

    param::value_type
    module::read(const std::string& var, bool hw)
    {
        auto mvi = std::find_if(module_var_descriptors.begin(),
                                module_var_descriptors.end(),
                                [var](param::module_var_desc& desc){
                                    return desc.name == var;
                                });
        if (mvi == module_var_descriptors.end()) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module param not found: " + var);
        }
        return read((*mvi).var, hw);

    }

    param::value_type
    module::read(param::module_var var, bool hw)
    {
        size_t index = static_cast<size_t>(var);
        if (index >= module_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid module variable: " << index;
            throw error(number, slot,
                        error::code::module_invalid_param, oss.str());
        }
        auto& desc = module_var_descriptors[index];
        log(log::debug) << "module: read: " << desc.name;
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module variable disabled: " + desc.name);
        }
        if (desc.mode == param::wr) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module variable not readable: " + desc.name);
        }
        lock_guard guard(lock_);
        param::value_type value;
        if (hw) {
            hw::dsp::memory mem = dsp.read(desc.address);
            hw::dsp::convert(mem, value);
            module_values[index] = value;
        } else {
            value = module_values[index];
        }
        return value;
    }

    param::value_type
    module::read(const std::string& var, size_t channel, bool hw)
    {
        auto cvi = std::find_if(channel_var_descriptors.begin(),
                                channel_var_descriptors.end(),
                                [var](param::channel_var_desc& desc){
                                    return desc.name == var;
                                });
        if (cvi == channel_var_descriptors.end()) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel param not found: " + var);
        }
        return read((*cvi).var, channel, hw);

    }

    param::value_type
    module::read(param::channel_var var, size_t channel, bool hw)
    {
        if (channel >= num_channels) {
            std::ostringstream oss;
            oss << "invalid channel number: " << channel;
            throw error(number, slot,
                        error::code::channel_number_invalid, oss.str());
        }
        size_t index = static_cast<size_t>(var);
        if (index >= channel_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid channel variable: " << index;
            throw error(number, slot,
                        error::code::channel_invalid_param, oss.str());
        }
        auto& desc = channel_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel variable disabled: " + desc.name);
        }
        if (desc.mode == param::wr) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel variable not readable: " + desc.name);
        }
        lock_guard guard(lock_);
        param::value_type value;
        if (hw) {
            hw::dsp::convert(dsp.read(channel, desc.address), value);
            channel_values[channel][index] = value;
        } else {
            value = channel_values[channel][index];
        }
        return value;
    }

    void
    module::write(const std::string& var, param::value_type value, bool hw)
    {
        auto mvi = std::find_if(module_var_descriptors.begin(),
                                module_var_descriptors.end(),
                                [var](param::module_var_desc& desc){
                                    return desc.name == var;
                                });
        if (mvi == module_var_descriptors.end()) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module param not found: " + var);
        }
        write((*mvi).var, value, hw);
    }

    void
    module::write(param::module_var var, param::value_type value, bool hw)
    {
        size_t index = static_cast<size_t>(var);
        if (index >= module_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid module variable: " << index;
            throw error(number, slot,
                        error::code::module_invalid_param, oss.str());
        }
        auto& desc = module_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module variable disabled: " + desc.name);
        }
        if (desc.mode == param::ro) {
            throw error(number, slot,
                        error::code::module_invalid_param,
                        "module variable not writeable: " + desc.name);
        }
        lock_guard guard(lock_);
        if (hw) {
            hw::dsp::memory mem;
            hw::dsp::convert(value, mem);
            dsp.write(desc.address, mem);
        }
        module_values[index] = value;
    }

    void
    module::write(const std::string& var, size_t channel,
                  param::value_type value, bool hw)
    {
        auto cvi = std::find_if(channel_var_descriptors.begin(),
                                channel_var_descriptors.end(),
                                [var](param::channel_var_desc& desc){
                                    return desc.name == var;
                                });
        if (cvi == channel_var_descriptors.end()) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel param not found: " + var);
        }
        write((*cvi).var, channel, value, hw);
    }

    void
    module::write(param::channel_var var, size_t channel,
                  param::value_type value, bool hw)
    {
        if (channel >= num_channels) {
            std::ostringstream oss;
            oss << "invalid channel number: " << channel;
            throw error(number, slot,
                        error::code::channel_number_invalid, oss.str());
        }
        size_t index = static_cast<size_t>(var);
        if (index >= channel_var_descriptors.size()) {
            std::ostringstream oss;
            oss << "invalid channel variable: " << index;
            throw error(number, slot,
                        error::code::channel_invalid_param, oss.str());
        }
        auto& desc = channel_var_descriptors[index];
        if (desc.state == param::disable) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel variable disabled: " + desc.name);
        }
        if (desc.mode == param::ro) {
            throw error(number, slot,
                        error::code::channel_invalid_param,
                        "channel variable not writeable: " + desc.name);
        }
        lock_guard guard(lock_);
        if (hw) {
            hw::dsp::memory mem;
            hw::dsp::convert(value, mem);
            dsp.write(channel, desc.address, mem);
        }
        channel_values[channel][index] = value;
    }

    void
    module::output(std::ostream& out) const {
        ostream_guard flags(out);
        out << std::boolalpha
            << "number: " << number
            << " slot: " << slot
            << " present:" << present_
            << " online:" << online_
            << " serial:" << serial_num
            << " rev:" << revision_label()
            << " (" << revision
            << ") vaddr:" << vmaddr
            << " fw: " << firmware.size();
    }

    char
    module::revision_label() const
    {
        return static_cast<char>(revision + 55);
    }

    bool
    module::eeprom_v2() const
    {
        return false;
    }

    void
    assign(modules& modules_, const number_slots& numbers)
    {
        for (auto number_slot : numbers) {
            for (auto& mod : modules_) {
                if (mod.slot == number_slot.second) {
                    mod.number = number_slot.first;
                    break;
                }
            }
        }
    }

    void
    order_by_number(modules& mods)
    {
        std::sort(mods.begin(),
                  mods.end(),
                  [](module& a, module& b) {
                      return a.number < b.number; } );
    }

    void
    order_by_slot(modules& mods)
    {
        std::sort(mods.begin(),
                  mods.end(),
                  [](module& a, module& b) {
                      return a.slot < b.slot; } );
    }

    void
    set_number_by_slot(modules& mods)
    {
        order_by_slot(mods);
        int number = 0;
        for (auto& mod : mods) {
            mod.number = number;
            number++;
        }
    }
};
};
};

std::ostream&
operator<<(std::ostream& out, const xia::pixie::module::module& module)
{
    module.output(out);
    return out;
}
