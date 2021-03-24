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
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>

#include <pixie_fw.hpp>

namespace xia
{
namespace pixie
{
namespace firmware
{
    error::error(const std::string& what)
        : runtime_error(what) {
    }

    error::error(const char* what)
        : runtime_error(what) {
    }

    firmware::firmware(const std::string version_,
                       const int mod_revision_,
                       const std::string device_)
        : version(version_),
          mod_revision(mod_revision_),
          device(device_)
    {
    }

    void
    firmware::load()
    {
        std::ifstream file(filename, std::ios::binary);

        file.unsetf(std::ios::skipws);
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        size_t size = ((size_t(file_size) - 1) / sizeof(image_value_type)) + 1;

        data.reserve(size * sizeof(image_value_type));

        data.assign((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
    }

    void
    firmware::clear()
    {
        data.clear();
        data.shrink_to_fit();
    }

    size_t
    firmware::words() const
    {
        /*
         * Size of the vector should be uint32_t aligned so rounding there
         * should redundant but it does not harm so lets keep it.
         */
        return ((size_t(data.size()) - 1) / sizeof(image_value_type)) + 1;
    }

    bool
    firmware::operator==(const firmware& fw) const
    {
        return
            fw.version == version &&
            fw.mod_revision == mod_revision &&
            fw.device == device;
    }

    void
    firmware::output(std::ostream& out) const {
        out << "ver:" << version
            << " mod-rev:" << mod_revision
            << " dev:" << device
            << " slots:" << slot.size() << ':';
        if (slot.size() > 0) {
            const char* delimiter = "";
            for (auto s : slot) {
                out << delimiter << s;
                delimiter = ",";
            }
        } else {
            out << "default";
        }
        out << " size:" << data.size();
    }

    void
    add(crate& firmwares, firmware& fw)
    {
        if (firmwares.find(fw.mod_revision) == std::end(firmwares)) {
            firmwares[fw.mod_revision] = module();
        }
        firmwares[fw.mod_revision].push_back(std::make_shared<firmware>(fw));
    }

    bool
    check(const crate& firmwares, const firmware& fw)
    {
        auto mod_fw = firmwares.find(fw.mod_revision);
        if (mod_fw != firmwares.end()) {
            for (auto fwr : std::get<1>(*mod_fw)) {
                if (*fwr == fw) {
                    return true;
                }
            }
        }
        return false;
    }

    firmware_ref
    find(module& firmwares, const std::string device, const int slot)
    {
        /*
         * First check if a slot assigned firmware exists for this
         * device. If not see if a default is available.
         */
        for (auto& fwr : firmwares) {
            if (fwr->device == device && !fwr->slot.empty()) {
                auto found = std::find(fwr->slot.begin(),
                                       fwr->slot.end(),
                                       slot);
                if (found != fwr->slot.end()) {
                    return fwr;
                }
            }
        }
        for (auto& fwr : firmwares) {
            if (fwr->device == device && fwr->slot.empty()) {
                return fwr;
            }
        }
        std::string what("firmware not found: device=");
        what += device;
        throw error(what);
    }

    void
    load(crate& crate_fw)
    {
        for (auto& mod_fw : crate_fw) {
            load(mod_fw.second);
        }
    }

    void
    clear(crate& crate_fw)
    {
        for (auto& mod_fw : crate_fw) {
            clear(mod_fw.second);
        }
    }

    void
    load(module& mod_fw)
    {
        for (auto fwr : mod_fw) {
            fwr->load();
        }
    }

    void
    clear(module& mod_fw)
    {
        for (auto fwr : mod_fw) {
            fwr->clear();
        }
    }

    firmware
    parse(const std::string fw_desc, const char delimiter)
    {
        std::string fwd = fw_desc;

        if (delimiter == ' ') {
            std::transform(fwd.begin(), fwd.end(), fwd.begin(),
                           [](unsigned char c) -> unsigned char {
                               if (std::isspace(c)) return ' ';
                               return c; });
        }

        std::istringstream field_stream(fwd);
        std::string field;

        std::string version;
        if (std::getline(field_stream, field, delimiter)) {
            version = field;
        } else {
            std::string what("firmware version not found: ");
            what += fw_desc;
            throw error(what);
        }

        int mod_revision;
        if (std::getline(field_stream, field, delimiter)) {
            try {
                mod_revision = std::stoi(field);
            } catch (std::invalid_argument& e) {
                std::string what("firmware module revision not a number: ");
                what += field;
                throw error(what);
            }
        } else {
            std::string what("firmware module revision not found ");
            what += fw_desc;
            throw error(what);
        }

        std::string device;
        if (std::getline(field_stream, field, delimiter)) {
            device = field;
        } else {
            std::string what("firmware device found: ");
            what += fw_desc;
            throw error(what);
        }

        std::string filename;
        if (std::getline(field_stream, field, delimiter)) {
            filename = field;
        } else {
            std::string what("firmware filename not found: ");
            what += fw_desc;
            throw error(what);
        }

        firmware fw(version, mod_revision, device);
        fw.filename = filename;

        return fw;
    }
};
};
};

std::ostream&
operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw)
{
    fw.output(out);
    return out;
}

std::ostream&
operator<<(std::ostream& out, const xia::pixie::firmware::module& mod_fw)
{
    for (auto& mod : mod_fw) {
        out << *mod;
    }
    return out;
}
