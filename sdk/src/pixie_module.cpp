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

#include <iostream>
#include <sstream>

#include <pixie_module.hpp>

#include <PlxApi.h>

namespace xia
{
namespace pixie
{
namespace module
{
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

    bus_handle
    make_bus_handle()
    {
        return std::make_unique<pci_bus_handle>();
    }

    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
    }

    module::module(bus_handle& bus_)
        : bus(std::move(bus_)),
          slot(0),
          index(-1),
          vmaddr(0),
          module_var_descriptors(param::get_module_var_descriptors()),
          channel_var_descriptors(param::get_channel_var_descriptors())
    {
    }

    module::~module()
    {
        /*
         * Lets not throw from a desctructor, just log any errors.
         */
        if (bus) {
            PLX_STATUS ps;
            bus_handle bus_ = std::move(bus);

            if (vmaddr != 0) {
                ps = ::PlxPci_PciBarUnmap(&bus_->handle, &vmaddr);
                if (ps != PLX_STATUS_OK) {
                std::cout << "error: Pixie PCI BAR unmap: "
                          << bus_->device_number << " : " << ps
                          << std::endl;
                }
            }

            ps = ::PlxPci_DeviceClose(&bus_->handle);
            if (ps != PLX_STATUS_OK) {
                /*
                 * Lets not throw from a desctructor, just log the issue.
                 */
                std::cout << "error: Pixie PCI close: "
                          << bus_->device_number << " : " << ps
                          << std::endl;
            }
        }
    }

    void
    module::open()
    {
        PLX_STATUS ps = ::PlxPci_DeviceOpen(&bus->key, &bus->handle);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI open: device: " << bus->device_number
                << " : " << ps;
            throw error(oss.str());
        }

        /*
         * For PLX 9054, Space 0 is at PCI BAR 2.
         */
        ps = PlxPci_PciBarMap(&bus->handle, 2, (VOID**) &vmaddr);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI BAR map: device: " << bus->device_number
                << " : " << ps;
            throw error(oss.str());
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

    bool
    pci_find_module(int device_number, bus_handle& bus)
    {
        bus->device_number = -1;
        PLX_STATUS ps = ::PlxPci_DeviceFind(&bus->key, device_number);
        if (ps == PLX_STATUS_OK) {
            bus->device_number = device_number;
        }
        return ps == PLX_STATUS_OK;
    }
};
};
};
