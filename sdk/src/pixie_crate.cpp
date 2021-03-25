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

#include <algorithm>
#include <sstream>

#include <pixie_crate.hpp>

namespace xia
{
namespace pixie
{
namespace crate
{
    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
    }

    crate::crate(size_t num_modules_)
        : num_modules(num_modules_)
    {
    }

    crate::~crate()
    {
    }

    void
    crate::initialize(bool reg_trace)
    {
        size_t max_modules = num_modules;

        if (max_modules == 0) {
            max_modules = slots;
        }

        for (size_t device_number = 0;
             device_number < max_modules;
             ++device_number) {

            modules.push_back(module::module());

            module::module& module = modules.back();

            module.reg_trace = reg_trace;

            try {
                module.open(device_number);
            } catch (std::runtime_error& e) {
                if (module.present) {
                    std::cout << "module: device " << device_number
                              << ": error: " << e.what()
                              << std::endl;
                }
            }

            if (module.present) {
                std::cout << "module: device " << device_number
                          << ": slot:" << module.slot
                          << " serial-number:" << module.serial_num
                          << " revision:" << module.revision
                          << std::endl;
            } else {
                modules.pop_back();
            }
        }

        if (num_modules == 0) {
            num_modules = modules.size();
        }
    }

    void
    crate::boot()
    {
        firmware::load(firmware);
        for (auto& module : modules) {
            if (module.revision != 0) {
                module.boot();
            }
        }
        firmware::clear(firmware);
    }

    void
    crate::set(firmware::crate& firmwares)
    {
        firmware = firmwares;
        for (auto& module : modules) {
            auto mod_fw = firmware.find(module.revision);
            if (mod_fw != firmware.end()) {
                auto& fw = firmware[module.revision];
                std::copy(fw.begin(), fw.end(),
                          std::back_inserter(module.firmware));
            }
        }
    }

    void
    crate::output(std::ostream& out) const {
        out << "fw:" << firmware.size() << std::endl;
        for (auto fw : firmware) {
            out << ' ' << std::get<0>(fw) << ' ' << std::get<1>(fw)
                << std::endl;
        }
        out << "modules:" << modules.size() << std::endl;
        bool first = true;
        for (auto& mod : modules) {
            if (first) {
                first = false;
            } else {
                out << std::endl;
            }
            out << ' ' << mod;
        }
    }
};
};
};

std::ostream&
operator<<(std::ostream& out, const xia::pixie::crate::crate& crate)
{
    crate.output(out);
    return out;
}
