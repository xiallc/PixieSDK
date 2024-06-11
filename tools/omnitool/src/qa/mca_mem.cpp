/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief MCA Memory Test
 */

#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/utils/io.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>

namespace xia {
namespace omnitool {
namespace qa {

static const pixie::hw::words profile1{
    0x00000000,
    0xFFFFFFFF,
    0x11111111,
    0xEEEEEEEE,
    0x22222222,
    0xDDDDDDDD,
    0x33333333,
    0xCCCCCCCC,
    0x44444444,
    0xBBBBBBBB,
    0x55555555,
    0xAAAAAAAA,
    0x66666666,
    0x99999999,
    0x77777777,
    0x88888888
};

static const pixie::hw::words profile2{
    0xFFFFFFFF,
    0x00000000,
    0xEEEEEEEE,
    0x11111111,
    0xDDDDDDDD,
    0x22222222,
    0xCCCCCCCC,
    0x33333333,
    0xBBBBBBBB,
    0x44444444,
    0xAAAAAAAA,
    0x55555555,
    0x99999999,
    0x66666666,
    0x88888888,
    0x77777777
};

static size_t legacy_mca_test(const pixie::hw::words& profile_write,
    pixie::hw::memory::mca& mca, pixie::hw::address base, pixie::hw::address top) {
    size_t count = 0;
    for (pixie::hw::address addr = base; addr < top;
         addr += (pixie::hw::address)profile_write.size()) {
        mca.write(addr, profile_write);
    }
    for (pixie::hw::address addr = base; addr < top;
         addr += (pixie::hw::address)profile_write.size()) {
        pixie::hw::words profile_read(profile_write.size());
        mca.read(addr, profile_read);
        for (size_t i = 0; i < profile1.size(); i++) {
            if (profile_read[i] != profile_write[i]) {
                count++;
            }
        }
    }
    return count;
}

void mca_mem(command::context& context) {
    util::io::ostream_guard guard(context.opts.out);
    auto& crate = context.crate;
    command::module_range mod_nums;
    bool first = true;
    bool legacy = false;
    bool alias = false;
    bool bitwalk = false;
    bool checker = false;
    while (context.cmd.has_arg()) {
        auto opt = context.cmd.get_arg();
        if (opt.empty()) {
            break;
        }
        if (first) {
            first = false;
            command::modules_option(
                mod_nums, opt, crate.get_modules());
            continue;
        }
        if (opt == "legacy") {
            legacy = true;
        } else if (opt == "alias") {
            alias = true;
        } else if (opt == "bitwalk") {
            bitwalk = true;
        } else if (opt == "checker") {
            checker = true;
        } else {
            throw std::runtime_error("mca_mem: invalid option: " + opt);
        }
    }

    if (legacy == false && alias == false && bitwalk == false && checker == false) {
        legacy = true;
        alias = true;
        bitwalk = true;
        checker = true;
    }

    crate->ready();
    for (auto mod : mod_nums) {
        crate[mod].online_check();
        crate[mod].run_check();
    }

    for (auto mod : mod_nums) {
        context.opts.out << "Running MCA Memory test on slot "
                         << std::dec << mod << std::endl;
        pixie::module::module& module = crate[mod];
        size_t block_size = module.max_histogram_length;
        const pixie::hw::address base = pixie::hw::memory::HISTOGRAM_MEMORY;
        const pixie::hw::address top = pixie::hw::memory::HISTOGRAM_MEMORY
            + pixie::hw::memory::MCA_MEMORY_LENGTH;
        pixie::hw::memory::mca mca(module);
        context.opts.out << "Memory range (Slot " << module.slot
                         << "):" << " Base: " << std::showbase << std::hex
                         << base << " Top: " << top << std::endl;

        /* Legacy MCA Memory test */
        if (legacy) {
            std::stringstream stream;
            context.opts.out << "Running legacy" << std::endl;
            size_t count = 0;
            for (int i = 0; i < 20; i++) {
                const pixie::hw::words& profile = (i%2) ? profile2 : profile1;
                count += legacy_mca_test(profile, mca, base, top);
            }

            if (count != 0) {
                stream << "Failed legacy: errors: ";
            } else {
                stream << "Passed legacy";
            }
            context.opts.out << stream.str() << std::endl;
        }

        /* Bit walk */
        if (bitwalk) {
            bool passed = true;
            context.opts.out << "Running bitwalk" << std::endl;
            pixie::hw::words write_word(1);
            pixie::hw::words read_word(1);
            pixie::hw::word word = 1;
            for (size_t i = 0; i < std::numeric_limits<pixie::hw::word>::digits; i++) {
                word <<= 1;
                write_word[0] = word;
                mca.write(base, write_word);
                mca.read(base, read_word);
                if (write_word[0] != read_word[0]) {
                    passed = false;
                }
            }
            if (passed) {
                context.opts.out << "Passed bitwalk" << std::endl;
            } else {
                context.opts.out << "Failed bitwalk" << std::endl;
            }
        }

        /* Aliasing */
        if (alias) {
            bool passed = true;
            std::stringstream reason;
            reason << "Failed alias: ";
            context.opts.out << "Running alias" << std::endl;
            /* Zero the memory */
            pixie::hw::words buff(block_size, 0);
            for (pixie::hw::address addr = base; addr < top;
                 addr += (pixie::hw::address)block_size) {
                mca.write(addr, buff);
            }

            /* Check for zeroed memory */
            for (pixie::hw::address addr = base; addr < top;
                 addr += (pixie::hw::address)block_size) {
                mca.read(addr, buff);
                for (auto word : buff) {
                    if (word != 0x00000000) {
                        passed = false;
                        reason << "memory not able to be zeroed";
                    }
                }
            }

            /* Check for memory aliasing (address line check) */
            const pixie::hw::word test_word = 0xAABBCCDD;
            mca.write(base, &test_word, 1);

            /* Check for aliases */
            for (pixie::hw::address addr = base; addr < top;
                 addr += (pixie::hw::address)block_size) {
                mca.read(addr, buff);
                for (size_t i = 0; i < block_size; i++) {
                    if (addr == base && i == 0) {
                        if (buff[i] != test_word && passed) {
                            passed = false;
                            reason << "memory failed to write at address "
                                   << std::showbase <<std::hex << (addr + i);
                        }
                    } else if (buff[i] != 0x00000000 && passed) {
                        passed = false;
                        reason << "memory aliasing at address 0x" << std::hex
                               << (addr + i) << " with 0x" << (buff[i]);
                    }
                }
            }
            if (passed) {
                context.opts.out << "Passed alias" << std::endl;
            } else {
                context.opts.out << reason.str() << std::endl;
            }
        }

        /* Checkerboard test */
        if (checker) {
            bool passed = true;
            context.opts.out << "Running checker" << std::endl;
            const pixie::hw::word checker = 0x55555555;
            pixie::hw::words buff(block_size, checker);
            pixie::hw::address addr;
            for (addr = base; addr <= (top - block_size);
                 addr += (pixie::hw::address)block_size) {
                mca.write(addr, buff);
            }
            if (addr < top) {
                size_t shortfall = top - addr;
                mca.write(addr, buff.data(), shortfall);
            }
            for (addr = base; addr <= (top - block_size);
                 addr += (pixie::hw::address)block_size) {
                mca.read(addr, buff);
                for (size_t i = 0; i < block_size; i++) {
                    if (buff[i] != checker) {
                        passed = false;
                    }
                }
            }
            if (addr < top) {
                size_t shortfall = top - addr;
                mca.read(addr, buff.data(), shortfall);
                for (size_t i = 0; i < shortfall; i++) {
                    if (buff[i] != checker) {
                        passed = false;
                    }
                }
            }
            if (passed) {
                context.opts.out << "Passed checker" << std::endl;
            } else {
                context.opts.out << "Failed checker" << std::endl;
            }
        }
    }
}

void mca_mem_comp(
    command::context& context, command::completion& completions) {
    auto mca_mem_cmd = context.cmd.def;

    command::completions::modules_completions(context, mca_mem_cmd.name,
        1, completions);

    command::completions::argument_completion(
        {command::completion_entry::node::argument, "legacy", mca_mem_cmd.name, "", "legacy"},
        2, 5, completions);
    command::completions::argument_completion(
        {command::completion_entry::node::argument, "alias", mca_mem_cmd.name, "", "alias"},
        2, 5, completions);
    command::completions::argument_completion(
        {command::completion_entry::node::argument, "bitwalk", mca_mem_cmd.name, "", "bitwalk"},
        2, 5, completions);
    command::completions::argument_completion(
        {command::completion_entry::node::argument, "checker", mca_mem_cmd.name, "", "checker"},
        2, 5, completions);
}
}
}
}
