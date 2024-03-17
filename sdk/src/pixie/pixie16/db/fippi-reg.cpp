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

/** @file fippi-reg.cpp
 * @brief Implements FIPPI register driver
 */

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <pixie/pixie16/db/db.hpp>
#include <pixie/pixie16/db/fippi-reg.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
namespace fippi {
using error = error::error;
reg::reg(
    const std::string& name_, db& board_, reg::word reg_)
    : board(board_), name(name_) {
    reg_address = hw::fippi_addr(board.number.get<int>(), reg_);
    init_shadow();
}

reg::reg(
    const char* name_, db& board_, reg::word reg_)
    : board(board_), name(name_) {
    reg_address = hw::fippi_addr(board.number.get<int>(), reg_);
    init_shadow();
}

reg::reg(
    const std::string& name_, db& board_, size_t channel, reg::word reg_)
    : board(board_), name(name_) {
    reg_address = hw::fippi_addr(
        board.number.get<int>(), static_cast<int>(channel), reg_);
    init_shadow();
}

reg::reg(
    const char* name_, db& board_, size_t channel, reg::word reg_)
    : board(board_), name(name_) {
    reg_address = hw::fippi_addr(
        board.number.get<int>(), static_cast<int>(channel), reg_);
    init_shadow();
}

reg::~reg() {
    shadow.remove();
}

void reg::write(reg::word value) {
    hw::memory::fippi fippi_dev(board.module_);
    log(log::debug) << db_label(board) << std::hex
                    << "fippi: " << name << ": write: value=0x" << value
                    << " (addr=0x" << reg_address << ')';
    fippi_dev.write(reg_address, value);
    shadow.unlock_writes();
    shadow = value;
    shadow.lock_writes();
}

void reg::write(reg::word value, word mask) {
    write((shadow.get<word>() & ~mask) | (value & mask));
}

void reg::write(reg::word value, size_t base, size_t bits) {
    auto mask = ((static_cast<word>(1) << bits) - 1) << base;
    write((value << base) & mask, mask);
}

reg::word reg::read() {
    return shadow.get<word>();
}

reg::word reg::read(word mask) {
    return read() & ~mask;
}

reg::word reg::read(size_t base, size_t bits) {
    return (read() >> base) & ((static_cast<word>(1) << bits) - 1);
}

void reg::enable_mibs() {
    shadow.enable();
}

void reg::disable_mibs() {
    shadow.disable();
}

void reg::init_shadow() {
    shadow = mib::node(
        board.get_mib_base() + "fippi" + mib::mibsep + "reg" + mib::mibsep + name,
        mib::type::uinteger);
    shadow = mib::hint::fmt_hex;
    shadow = 0;
    shadow.lock_writes();
}

void make(regs& regs, const std::string& name, db& board, reg::address reg_) {
    auto search = regs.find(name);
    if (search != regs.end()) {
        throw error(error::code::invalid_value, "duplicate fippi reg: " + name);
    }
    regs[name] = std::make_shared<reg>(name, board, reg_);
}

void make(
    regs& regs, const std::string& name, db& board, size_t channel, reg::address reg_) {
    auto search = regs.find(name);
    if (search != regs.end()) {
        throw error(error::code::invalid_value, "duplicate fippi reg: " + name);
    }
    regs[name] = std::make_shared<reg>(name, board, channel, reg_);
}

reg_ptr find(const std::string& name, regs& regs) {
    return find(name.c_str(), regs);
}

reg_ptr find(const char* name, regs& regs) {
    auto search = regs.find(name);
    if (search == regs.end()) {
        std::string msg = "invalid fippi reg: ";
        msg += name;
        throw error(error::code::invalid_value, msg);
    }
    return search->second;
}

void enable_mibs(regs& regs) {
    for (auto r : regs) {
        r.second->enable_mibs();
    }
}

void disable_mibs(regs& regs) {
    for (auto r : regs) {
        r.second->disable_mibs();
    }
}
};  // namespace fippi
};  // namespace db
};  // namespace fixture
};  // namespace pixie
};  // namespace xia
