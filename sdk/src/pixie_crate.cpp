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
#include <iomanip>
#include <sstream>

#include <pixie_crate.hpp>
#include <pixie_log.hpp>

namespace xia
{
namespace pixie
{
namespace crate
{
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
            throw error(pixie::error::code::crate_not_ready,
                        "crate is not ready");
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

    void
    crate::initialize(size_t num_modules_, bool reg_trace)
    {
        const bool autodetect = num_modules_ == 0;

        log(log::info) << "crate: initialise: num-modules=" << num_modules_
                       << " auto-detect=" << std::boolalpha << autodetect;

        if (ready_.exchange(true)) {
            throw error(pixie::error::code::crate_already_open,
                        "create already initialised");
        }

        try {
            size_t max_modules = num_modules = num_modules_;

            if (max_modules == 0) {
                max_modules = hw::max_slots;
            }

            for (size_t device_number = 0;
                 device_number < max_modules;
                 ++device_number) {

                add_module();

                auto& module_ptr = modules.back();
                module::module& module = *module_ptr;
                bool last_module_found = false;

                try {
                    module.module_var_descriptors =
                        param::module_var_descs(param::get_module_var_descriptors());
                    module.channel_var_descriptors =
                        param::channel_var_descs(param::get_channel_var_descriptors());
                    module.reg_trace = reg_trace;
                    module.open(device_number);
                } catch (module::error& e) {
                    if (!autodetect) {
                        throw;
                    }
                    last_module_found = e.type == module::error::code::not_supported;
                    log(log::error) << "module: device " << device_number
                                    << ": error: " << e.what();
                }

                if (module.present()) {
                    log(log::info) << "module: device " << device_number
                                   << ": slot:" << module.slot
                                   << " serial-number:" << module.serial_num
                                   << " revision:" << module.revision_label();
                } else {
                    modules.pop_back();
                    if (last_module_found) {
                      break;
                    }
                }
            }

            module::set_number_by_slot(modules);

            if (num_modules == 0) {
                num_modules = modules.size();
            }
        } catch (...) {
            ready_ = false;
            throw;
        }
    }

    void
    crate::probe()
    {
        ready();
        firmware::load(firmware);
        for (auto& module : modules) {
            module->probe();
        }
        firmware::clear(firmware);
    }

    void
    crate::boot()
    {
        ready();
        firmware::load(firmware);
        for (auto& module : modules) {
            if (module->revision != 0) {
                module->boot();
            }
        }
        firmware::clear(firmware);
    }

    void
    crate::set_firmware()
    {
        ready();
        for (auto& module : modules) {
            for (auto& config : module->configs) {
                auto tag = firmware::tag(module->revision,
                                         config.adc_msps,
                                         config.adc_bits);
                auto mod_fw = firmware.find(tag);
                if (mod_fw != firmware.end()) {
                    log(log::debug) << module::module_label(*module)
                                    << "crate: add module firmware(s): "
                                    << tag;
                    module->add(firmware[tag]);
                } else {
                    log(log::debug) << module::module_label(*module)
                                    << "crate: module firmware already set: "
                                    << tag;
                }
            }
        }
    }

    void
    crate::output(std::ostream& out) const {
        if (!ready_.load()) {
            out << "not initialized";
            return;
        }
        out << "fw: tags: " << firmware.size() << std::endl;
        int c = 0;
        for (auto fw_rev : firmware) {
            for (auto& fw : std::get<1>(fw_rev)) {
                out << ' ' << std::setw(3) << ++c << ". "
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
            out << ' ' << *mod;
        }
    }

    void
    crate::assign(const module::number_slots& numbers)
    {
        ready();
        module::assign(modules, numbers);
        module::order_by_number(modules);
    }

    void
    crate::add_module()
    {
        modules.push_back(std::make_unique<module::module>());
    }

    module_handle::module_handle(crate& crate_, size_t number)
        : handle(crate_[number]),
          user(crate_),
          guard(handle)
    {
        crate_.ready();
        if (!handle.online()) {
            throw error(pixie::error::code::module_offline,
                        "module not online");
        }
    }

    module_handle::module_handle(crate& crate_, unsigned short number)
        : handle(crate_[number]),
          user(crate_),
          guard(handle)
    {
        crate_.ready();
        if (!handle.online()) {
            throw error(pixie::error::code::module_offline,
                        "module not online");
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
