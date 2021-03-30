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
#include <pixie_util.hpp>

#include <hw/dsp.hpp>
#include <hw/fpga_comms.hpp>
#include <hw/fpga_fippi.hpp>
#include <hw/i2cm24c64.hpp>
#include <hw/pcf8574.hpp>

#include <pixie16sys_defs.h>

#include <PlxApi.h>

namespace xia
{
namespace pixie
{
namespace module
{
    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
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

    module::guard::guard(module& mod)
        : guard_(mod.lock_)
    {
    }

    module::module()
        : slot(0),
          serial_num(0),
          revision(0),
          index(-1),
          vmaddr(nullptr),
          dsp(*this),
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
    }

    module::module(module&& m)
        : slot(m.slot),
          serial_num(m.serial_num),
          revision(m.revision),
          index(m.index),
          vmaddr(m.vmaddr),
          dsp(*this),
          module_var_descriptors(std::move(m.module_var_descriptors)),
          channel_var_descriptors(std::move(m.channel_var_descriptors)),
          reg_trace(m.reg_trace),
          in_use(0),
          present_(m.present_),
          online_(m.online_),
          comms_fpga(m.comms_fpga),
          fippi_fpga(m.fippi_fpga),
          device(std::move(m.device))
    {
        m.slot = 0;
        m.serial_num = 0;
        m.revision = 0;
        m.index = -1;
        m.vmaddr = nullptr;
        m.reg_trace = false;
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
            std::cout << "error: " << e.what() << std::endl;
        }
        device.release();
    }

    module&
    module::operator=(module&& m)
    {
        lock_guard guard(lock_);
        lock_guard guard_m(m.lock_);

        if (in_use != 0 || m.in_use != 0) {
            throw error("cannot move modules when in use");
        }

        slot = m.slot;
        serial_num = m.serial_num;
        revision = m.revision;
        index = m.index;
        vmaddr = m.vmaddr;
        dsp = std::move(m.dsp);
        module_var_descriptors = std::move(m.module_var_descriptors);
        channel_var_descriptors = std::move(m.channel_var_descriptors);
        reg_trace = m.reg_trace;
        present_ = m.present_;
        online_ = m.online_;
        comms_fpga = m.comms_fpga;
        fippi_fpga = m.fippi_fpga;

        device = std::move(m.device);

        m.slot = 0;
        m.serial_num = 0;
        m.revision = 0;
        m.index = -1;
        m.vmaddr = nullptr;
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
        if (online_) {
            throw error("module already open");
        }

        if (device->device_number < 0) {
            PLX_STATUS ps;

            ps = ::PlxPci_DeviceFind(&device->key, device_number);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "pixie PCI find: device: " << device->device_number
                    << " : " << ps;
                throw error(oss.str());
            }

            device->device_number = device_number;

            present_ = true;

            ps = ::PlxPci_DeviceOpen(&device->key, &device->handle);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "pixie PCI open: device: " << device->device_number
                    << " : " << ps;
                throw error(oss.str());
            }

            /*
             * For PLX 9054, Space 0 is at PCI BAR 2.
             */
            ps = PlxPci_PciBarMap(&device->handle, 2, (VOID**) &vmaddr);
            if (ps != PLX_STATUS_OK) {
                std::ostringstream oss;
                oss << "pixie PCI BAR map: device: " << device->device_number
                    << " : " << ps;
                throw error(oss.str());
            }

            hw::i2c::i2cm24c64 eeprom(*this, I2CM24C64_ADDR,
                                      (1 << 0) | (1 << 3), 1 << 1, 1 << 2);
            std::vector<uint8_t> data;

            eeprom.sequential_read(0, 3, data);

            if (data.size() != 3) {
                std::ostringstream oss;
                oss << "pixie EEPROM read: device: " << device->device_number
                    << " : invalid data length:" << data.size();
                throw error(oss.str());
            }

            /*
             *  Starting with serial number 256, serial number is stored in the
             *  first two bytes of EEPROM, followed by revision number, which
             *  is at least 11 (i.e. Rev-B)
             */
            revision = data[2];

            if (revision >= 11) {
                serial_num = ((int) data[1] << 8) | (int) data[0];
            } else {
                serial_num = (unsigned int) data[0];
            }

            hw::i2c::pcf8574 pio(*this, PCF8574_ADDR,
                                 (1 << 0) | (1 << 3), 1 << 1, 1 << 2);

            slot = (pio.read_a_byte() & 0xf8) >> 3;
        }
    }

    void
    module::close()
    {
        if (device && device->device_number >= 0) {
            PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
            PLX_STATUS ps_close;

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
                throw error(oss.str());
            }
        }
    }

    void
    module::initialize(const std::string varsdef_)
    {
        varsdef = varsdef_;
        param::load(varsdef,
                    module_var_descriptors,
                    channel_var_descriptors);
    }

    void
    module::boot(bool boot_comms, bool boot_fippi, bool boot_dsp)
    {
        if (online_) {
            throw error("module is online");
        }

        if (boot_comms) {
            if (comms_fpga) {
                throw error("comms already booted");
            }
            firmware::firmware_ref fw = get("sys");
            hw::fpga::comms comms(*this);
            comms.boot(fw->data);
        }

        if (boot_fippi) {
            firmware::firmware_ref fw = get("fippi");
            hw::fpga::fippi fippi(*this);
            fippi.boot(fw->data);
        }

        if (boot_dsp) {
            firmware::firmware_ref fw = get("dsp");
            dsp.boot(fw->data);
        }

        online_ = comms_fpga && boot_fippi && dsp.online;
    }

    void
    module::set(firmware::module& fw)
    {
        if (online_) {
            throw error("module is online");
        }
        firmware.clear();
        std::copy(fw.begin(), fw.end(),
                  std::back_inserter(firmware));
    }

    firmware::firmware_ref
    module::get(const std::string device)
    {
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
        throw error(oss.str());
    }

    void
    assign(modules& modules_, const index_slots& indexes)
    {
        for (auto index_slot : indexes) {
            for (auto& mod : modules_) {
                if (mod.slot == index_slot.second) {
                    mod.index = index_slot.first;
                    break;
                }
            }
        }
    }

    void
    order_by_index(modules& mods)
    {
        std::sort(mods.begin(),
                  mods.end(),
                  [](module& a, module& b) {
                      return a.index < b.index; } );
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
    set_index_by_slot(modules& mods)
    {
        order_by_slot(mods);
        int index = 0;
        for (auto& mod : mods) {
            mod.index = index;
            index++;
        }
    }

    void
    module::output(std::ostream& out) const {
        ostream_guard flags(out);
        out << std::boolalpha
            << "slot: " << slot
            << " present:" << present_
            << " online:" << online_
            << " serial:" << serial_num
            << " rev:" << revision
            << " vaddr:" << vmaddr
            << " fw: " << firmware.size();
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
