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

/** @file fippi-reg.hpp
 * @brief A register driver for the FIPPI
 */

#ifndef PIXIE_DB_FIPPI_REG_H
#define PIXIE_DB_FIPPI_REG_H

#include <map>

#include <pixie/mib.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
struct db;
namespace fippi {
/**
 * @brief FIPPI Register Driver
 * There is an instance per daughter board to acesses a single FIPPI
 * register The driver is concurrent.
 */
struct reg {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;
    using address = hw::address;
    using word = hw::word;
    db& board;
    lock_type lock;
    std::string name;
    address reg_address;
    mib::node shadow;
    reg(const std::string& name, db& board, address reg);
    reg(const char* name, db& board, address reg);
    reg(const std::string& name, db& board, size_t channel, address reg);
    reg(const char* name, db& board, size_t channel, address reg);
    ~reg();
    void write(word value);
    void write(word value, word mask);
    void write(word value, size_t base, size_t bits);
    word read();
    word read(word mask);
    word read(size_t base, size_t bits);
    void enable_mibs();
    void disable_mibs();
private:
    void init_shadow();
};

/*
 * Table of named sharable registers
 */
using reg_ptr = std::shared_ptr<reg>;
using regs = std::map<std::string, reg_ptr>;

void make(regs& regs, const std::string& name, db& board, reg::address reg);
void make(
    regs& regs, const std::string& name, db& board, size_t channel, reg::address reg);

reg_ptr find(const std::string& name, regs& regs);
reg_ptr find(const char* name, regs& regs);

void enable_mibs(regs& regs);
void disable_mibs(regs& regs);
}  // namespace fippi
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_FIPPI_REG_H
