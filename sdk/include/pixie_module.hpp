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

#include <memory>
#include <stdexcept>
#include <vector>

#include <pixie_hw.hpp>
#include <pixie_param.hpp>

namespace xia
{
namespace pixie
{
namespace module
{
    /*
     * Module errors
     */
    class error
        : public std::runtime_error {
    public:
        explicit error(const std::string& what);
        explicit error(const char* what);
    };

    /*
     * PCI bus handle is opaque. No direct access as it is
     * specific to the PCI drivers.
     */
    struct pci_bus_handle;
    typedef std::unique_ptr<pci_bus_handle> bus_handle;

    bus_handle make_bus_handle();
    bool pci_find_module(int device_number, bus_handle& device);
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
    struct module
    {
        /*
         * The type is opaque.
         */
        bus_handle device;

        /*
         * Slot in the crate.
         */
        int slot;

        /*
         * Logical module mapping for this instance of the
         * SDK.
         */
        int index;

        /*
         * Module's register VM address.
         */
        void* vmaddr;

        param::module_var_descs module_var_descriptors;
        param::channel_var_descs channel_var_descriptors;

        std::string varsdef;

        module();
        module(module&& m);
        ~module();

        module& operator-(module&& m);

        void open();
        void close();

        void initialize(const std::string varsdef_);

        /*
         * IO read 32 bits value.
         */
        inline uint32_t read_32(int reg) {
            return hw::read_32(vmaddr, reg);
        }

        /*
         * IO write 32 bits value.
         */
        inline void write_32(int reg,
                             const uint32_t value) {
            hw::write_32(vmaddr, reg, value);
        }
    };

    typedef std::vector<module> modules;
}
}
}

#endif  // PIXIE_MODULE_H
