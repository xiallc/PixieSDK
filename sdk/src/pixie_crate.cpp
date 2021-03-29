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
#include <iomanip>
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

    crate::user::user(crate& crate__)
        : crate_(crate__)
    {
        ++crate_.users_;
    }

    crate::user::~user()
    {
        --crate_.users_;
    }

    crate::crate()
        : num_modules(0),
          ready_(false)
    {
    }

    crate::~crate()
    {
    }

    void
    crate::ready()
    {
        if (!ready_.load()) {
            throw error("crate is not ready");
        }
    }

    bool
    crate::busy() const
    {
        return users_.load() > 0;
    }

    int
    crate::users() const
    {
        return users_.load();
    }

    template<typename T> module::module&
    crate::operator[](T number_)
    {
        size_t number = static_cast<size_t>(number_);
        if (number >= num_modules) {
            throw error("number out of range");
        }
        return modules[number];
    }

    void
    crate::initialize(size_t num_modules_, bool reg_trace)
    {
        if (ready_.exchange(true)) {
            throw error("create already initialised");
        }

        try {
            size_t max_modules = num_modules = num_modules_;

            if (max_modules == 0) {
                max_modules = slots;
            }

            for (size_t device_number = 0;
                 device_number < max_modules;
                 ++device_number) {

                modules.push_back(module::module());

                module::module& module = modules.back();

                try {
                    module.open(device_number);
                    module.reg_trace = reg_trace;
                } catch (std::runtime_error& e) {
                    if (module.present()) {
                        std::cout << "module: device " << device_number
                                  << ": error: " << e.what()
                                  << std::endl;
                    }
                }

                if (module.present()) {
                    std::cout << "module: device " << device_number
                              << ": slot:" << module.slot
                              << " serial-number:" << module.serial_num
                              << " revision:" << module.revision
                              << std::endl;
                } else {
                    modules.pop_back();
                }
            }

            module::set_index_by_slot(modules);

            if (num_modules == 0) {
                num_modules = modules.size();
            }
        } catch (...) {
            ready_ = false;
            throw;
        }
    }

    void
    crate::boot()
    {
        ready();
        firmware::load(firmware);
        for (auto& module : modules) {
            if (module.revision != 0) {
                try {
                    module.boot();
                } catch (std::runtime_error& e) {
                    std::cout << "module [slot " << module.slot
                              << "]: " << e.what()
                              << std::endl;
                }
            }
        }
        firmware::clear(firmware);
    }

    void
    crate::set_firmware()
    {
        ready();
        for (auto& module : modules) {
            auto mod_fw = firmware.find(module.revision);
            if (mod_fw != firmware.end()) {
                module.set(firmware[module.revision]);
            }
        }
    }

    void
    crate::output(std::ostream& out) const {
        if (!ready_.load()) {
            out << "not initialized";
            return;
        }
        out << "fw: revs: " << firmware.size() << std::endl;
        int c = 0;
        for (auto fw_rev : firmware) {
            for (auto& fw : std::get<1>(fw_rev)) {
                out << ' ' << std::setw(3) << ++c << ". "
                    << std::get<0>(fw_rev)
                    << ' ' << *fw
                    << std::endl;
            }
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

    void
    crate::assign(const module::index_slots& indexes)
    {
        ready();
        module::assign(modules, indexes);
        module::order_by_index(modules);
    }

    module_handle::module_handle(crate& crate_, size_t number)
        : handle(crate_[number]),
          user(crate_),
          guard(handle)
    {
        crate_.ready();
        if (!handle.online()) {
            throw error("module not online");
        }
    }

    module_handle::module_handle(crate& crate_, unsigned short number)
        : handle(crate_[number]),
          user(crate_),
          guard(handle)
    {
        crate_.ready();
        if (!handle.online()) {
            throw error("module not online");
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
