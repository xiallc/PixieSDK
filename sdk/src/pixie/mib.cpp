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

/** @file mib.cpp
 * @brief Implements functions and data structures for the MIB
 */

#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include <pixie/log.hpp>
#include <pixie/mib.hpp>
#include <pixie/utils/string.hpp>
#include <pixie/utils/time.hpp>

namespace xia {
namespace mib {
static bool is_number(const char* s, size_t len) {
    while (*s != '\0' && len != 0) {
        if (!std::isdigit(*s)) {
            return false;
        }
        s++;
        len--;
    }
    return true;
}

static long get_number(const char* s) {
    return std::strtol(s, nullptr, 10);
}

static void next_field(
    const std::string& str, size_t& pos, size_t& end, size_t& len) {
    end = str.find(mibsep, pos);
    if (end == std::string::npos) {
        end = str.size();
    }
    len = end - pos;
}

/**
 * The MIB is held in a single map with each MIB node holding its
 * complete path. With the low number of nodes and the size of the
 * full paths it is simpler than adding complexity to hold the MIB as
 * a tree structure.
 */
struct mib_nodes {
    struct name_cmp {
        bool operator()(const name_type& lhs, const name_type& rhs) const {
            auto l_pieces =
                std::count_if(lhs.begin(), lhs.end(), [](char c) {
                    return c == mibsep; }) + 1;
            auto r_pieces =
                std::count_if(rhs.begin(), rhs.end(), [](char c) {
                    return c == mibsep; }) + 1;
            size_t l_p = 0;
            size_t r_p = 0;
            size_t pieces = std::min(l_pieces, r_pieces);
            for (size_t p = 0; p < pieces; ++p) {
                size_t l_end = 0;
                size_t l_len = 0;
                size_t r_end = 0;
                size_t r_len = 0;
                const char* ls = lhs.c_str() + l_p;
                const char* rs = rhs.c_str() + r_p;
                next_field(lhs, l_p, l_end, l_len);
                next_field(rhs, r_p, r_end, r_len);
                bool no_match = true;
                if (l_len == r_len) {
                    no_match = std::strncmp(ls, rs, l_len) != 0;
                }
                if (no_match) {
                    bool l_isnum = is_number(ls, l_len);
                    bool r_isnum = is_number(rs, r_len);
                    if (!l_isnum && !r_isnum) {
                        auto min = std::min(l_len, r_len);
                        auto result = std::strncmp(ls, rs, min);
                        if (result != 0) {
                            return result < 0;
                        }
                        return l_len <= r_len;
                    } else if (l_isnum && r_isnum) {
                        return get_number(ls) < get_number(rs);
                    } else if (l_isnum) {
                        return true;
                    } else {
                        return false;
                    }
                }
                l_p = l_end + 1;
                r_p = r_end + 1;
            }
            return l_pieces < r_pieces;
        }
    };

    using nodes_type = std::map<name_type, node_base_ptr, name_cmp>;
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;

    lock_type lock;
    nodes_type nodes;

    mib_nodes();
    ~mib_nodes() = default;

    mib_nodes(const mib_nodes& ) = delete;
    mib_nodes(const mib_nodes&& ) = delete;
    mib_nodes& operator=(const mib_nodes& ) = delete;

    node_base_ptr add(const name_type& name, const type type_, const bool enabled);
    node_base_ptr add(const char* name, const type type_, const bool enabled);
    template<typename T> node_base_ptr add(
        const name_type& name, const T& val, const bool enabled);
    void remove(node_base_ptr& base);
    node_base_ptr find(const name_type& name);
    node_base_ptr find(const char* name);
    bool contains(const name_type& name);
    void walk(mib_walk_func& walk_func);
};

/*
 * Access the mib as a pointer, ie `mib->` and no check needed. All
 * functions accessing the pointer must be declared in mib.hpp.
 */
mib_nodes_ptr mib;

PIXIE_EXPORT mib_nodes_ptr PIXIE_API make_mib_nodes() {
    if (!mib) {
        mib = std::make_shared<mib_nodes>();
    }
    return mib;
}

mib_nodes::mib_nodes() {}

node_base_ptr mib_nodes::add(const name_type& name, const type type_, const bool enabled) {
    if (name.empty()) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    lock_guard guard(lock);
    auto ni = nodes.emplace(name, std::make_shared<node_base>(name, type_, enabled));
    if (!ni.second) {
        throw error(
          error::code::invalid_value,
          "mib::add: mib already registered: " + name);
    }
    return (*ni.first).second;
}

node_base_ptr mib_nodes::add(const char* name, const type type_, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    return mib->add(name_type(name), type_, enabled);
}

template<typename T>
node_base_ptr mib_nodes::add(const name_type& name, const T& val, const bool enabled) {
    if (name.empty()) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    lock_guard guard(lock);
    auto ni = nodes.emplace(name, std::make_shared<node_base>(name, val, enabled));
    if (!ni.second) {
        throw error(
          error::code::invalid_value,
          "mib::add: mib already registered: " + name);
    }
    return (*ni.first).second;
}

void mib_nodes::remove(node_base_ptr& base) {
    lock_guard guard(lock);
    base->lock.lock();
    auto ni = nodes.find(base->name);
    if (ni == nodes.end()) {
        base->lock.unlock();
        throw error(
            error::code::internal_failure,
            "mib::remove: mib not found: " + base->name);
    }
    /*
     * Chekc if the base passed in is the one in the container. It
     * should never be possible but changes could let it happen.
     */
    if (base.use_count() > 2 ||
        (&base == &(*ni).second && base.use_count() > 1)) {
        base->lock.unlock();
        throw error(
            error::code::internal_failure,
            "mib::remove: mib base has references: " + base->name);
    }
    nodes.erase(ni);
    base->lock.unlock();
    base.reset();
}

node_base_ptr mib_nodes::find(const name_type& name) {
    if (name.empty()) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    lock_guard guard(lock);
    auto ni = nodes.find(name);
    if (ni == nodes.end()) {
        throw error(error::code::invalid_value, "mib::find: mib not found: " + name);
    }
    return (*ni).second;
}

node_base_ptr mib_nodes::find(const char* name) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    return find(std::string(name));
}

bool mib_nodes::contains(const name_type& name) {
    if (name.empty()) {
        return false;
    }
    lock_guard guard(lock);
    auto ni = nodes.find(name);
    return ni != nodes.end();
}

void mib_nodes::walk(mib_walk_func& walk_func) {
    lock_guard guard(lock);
    for (auto& [key, base] : mib->nodes) {
        auto n = node(base);
        if (n.is_enabled()) {
            walk_func(n);
        }
   }
}

data_type::data_type()
    : s() {
}

data_type::~data_type() {
}

node_base::node_base(const name_type& name_, const type type__, const bool enabled_)
    : name(name_), type_(type__), read_only(false), write_lock(false),
      enabled(enabled_), in_event_call(false), hints(0) {
    /*
     * A mib can only ever be one type. The union has classes and they
     * could contain references to resources such as memory. The
     * complexity to be able to change a MIB is not needed as a MIB is
     * specific to a single role defined at compile time.
     */
    switch (type_) {
    default:
        throw error(
            error::code::internal_failure, "mib::node: invalid type: " + name);
        break;
    case type::string:
        v.s = std::string();
        break;
    case type::boolean:
        v.b = false;
        break;
    case type::integer:
    case type::uinteger:
        v.u = 0ULL;
        break;
    case type::real:
        v.r = 0.0;
        break;
    case type::timestamp:
        v.t.nsecs = 0ULL;
        break;
    }
}

node_base::~node_base() {
    switch (type_) {
    case type::string:
        v.s.clear();
        v.s = std::string();
        break;
    default:
        break;
    }
}

bool node_base::operator==(const node_base& other) const {
    return name == other.name;
}

bool node_base::operator<(const node_base& other) const {
    return name < other.name;
}

void node_base::enable() {
    lock_guard guard(lock);
    enabled = true;
}

void node_base::disable() {
    lock_guard guard(lock);
    enabled = false;
}

bool node_base::is_enabled() {
    lock_guard guard(lock);
    return enabled;
}

void node_base::set_value(const std::string& val) {
    set_value(val.c_str());
}

void node_base::set_value(const char* val) {
    lock_guard guard(lock);
    if (read_only) {
        throw error(
            error::code::read_only, "mib::node::set: MIB is read-only: " + name);
    }
    if (write_lock) {
        throw error(
            error::code::read_only, "mib::node::set: MIB is write locked: " + name);
    }
    std::istringstream iss(val);
    iss >> std::skipws;
    iss.exceptions(std::ifstream::failbit);
    bool ok = false;
    switch (type_) {
    case type::string:
        v.s = val;
        ok = true;
        break;
    case type::boolean:
        try {
            iss >> v.b;
            ok = true;
        } catch (...) {
        }
        if (!ok) {
            iss.clear();
            iss.seekg(0);
            try {
                iss >> std::boolalpha >> v.b;
                ok = true;
            } catch (...) {
            }
        }
        break;
    case type::integer:
        try {
            integer i;
            iss >> std::setbase(0) >> i;
            v.u = i;
            ok = true;
        } catch (...) {
        }
        break;
    case type::uinteger:
        try {
            /*
             * A value such as -1 is converted and we want an
             * error. If the value is parsed without error parse again
             * as an integer and if that falis the value is unsigned
             * as it overflowed the integer. If it is parsed as an
             * integer make sure it is not negative.
             */
            iss >> std::setbase(0) >> v.u;
            try {
                iss.seekg(0);
                integer i;
                iss >> std::setbase(0) >> i;
                if (i >= 0) {
                    ok = true;
                }
            } catch (...) {
                ok = true;
            }
        } catch (...) {
        }
        break;
    case type::real:
        try {
            iss >> v.r;
            ok = true;
        } catch (...) {
        }
        break;
    case type::timestamp:
        #ifdef XIA_PIXIE_WINDOWS
            #define timegm _mkgmtime64
        #endif
        try {
            std::tm t = {};
            iss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
            auto tt = timegm(&t);
            v.t.nsecs = tt * 1000000000;
            iss.seekg(0);
            auto count = iss.str().size();
            while (count-- > 0) {
                char c;
                iss >> c;
                if (c == '.') {
                    uinteger u;
                    iss >> std::setbase(10) >> u;
                    uinteger scale = 1000;
                    if (count > 6) {
                        for (size_t c = 6; c < count; ++c) {
                            scale *= 10;
                        }
                    }
                    v.t.nsecs += u * scale;
                    break;
                }
            }
            ok = true;
        } catch (...) {
            throw;
        }
        break;
    default:
        throw error(
            error::code::internal_failure, "mib::node: invalid type: " + name);
        break;
    }
    if (!ok) {
        throw error(
            error::code::invalid_value,
            "mib::node: set: invalid value: " + name + ": " + val);
    }
    call_set_event_func();
}

void node_base::check(const type type__) {
    if (type_ != type__) {
        std::ostringstream oss;
        oss << "mib::node::check: " << name
            << ": invalid type: " << int(type_) << '/' << int(type__);
        throw error(error::code::invalid_value, oss.str());
    }
}

void node_base::set_event_func(event evt, event_func& func) {
    lock_guard guard(lock);
    if (read_only) {
        throw error(
            error::code::internal_failure,
            "mib::node::event: cannot set events on reead-only MIBs");
    }
    switch (evt) {
    case event::set:
        set_event = func;
        break;
    case event::get:
        get_event = func;
        break;
    case event::timer:
        timer_event = func;
        break;
    default:
        throw error(
            error::code::internal_failure,
            "mib::node::event: invlaid event type");
    }
}

void node_base::clear_event_func(event evt) {
    lock_guard guard(lock);
    if (read_only) {
        throw error(
            error::code::internal_failure,
            "mib::node::event: cannot set events on reead-only MIBs");
    }
    switch (evt) {
    case event::set:
        set_event = nullptr;
        break;
    case event::get:
        get_event = nullptr;
        break;
    case event::timer:
        timer_event = nullptr;
        break;
    default:
        throw error(
            error::code::internal_failure,
            "mib::node::event: invlaid event type");
    }
}

bool node_base::has_event_func(event evt) {
    lock_guard guard(lock);
    if (read_only) {
        throw error(
            error::code::internal_failure,
            "mib::node::event: cannot set events on reead-only MIBs");
    }
    bool has;
    switch (evt) {
    case event::set:
        has = (bool) set_event;
        break;
    case event::get:
        has = (bool) get_event;
        break;
    case event::timer:
        has = (bool) timer_event;
        break;
    default:
        throw error(
            error::code::internal_failure,
            "mib::node::event: invlaid event type");
    }
    return has;
}

void node_base::call_set_event_func() {
    if (set_event && !in_event_call) {
        event_call_guard event_guard(lock, in_event_call);
        set_event(event::set, type_, v);
    }
}

void node_base::call_get_event_func() {
    if (get_event && !in_event_call) {
        event_call_guard event_guard(lock, in_event_call);
        get_event(event::get, type_, v);
    }
}

void node_base::call_timer_event_func() {
    if (timer_event && !in_event_call) {
        event_call_guard event_guard(lock, in_event_call);
        timer_event(event::timer, type_, v);
    }
}

std::string node_base::str(bool attributes) {
    lock_guard guard(lock);
    call_get_event_func();
    std::ostringstream oss;
    oss << std::boolalpha
        << std::dec
        << std::showbase
        << std::fixed;
    bool s_fmt_no_quotes = false;
    bool ts_fmt_iso8601 = false;
    switch (hints & (fmt_mask << fmt_base)) {
    case fmt_no_quotes:
        s_fmt_no_quotes = true;
        break;
    case fmt_boolnum:
        oss << std::noboolalpha;
        break;
    case fmt_dec:
        break;
    case fmt_oct:
        oss << std::oct;
        break;
    case fmt_hex:
        oss << std::hex;
        break;
    case fmt_fixed:
        break;
    case fmt_iso8601:
        ts_fmt_iso8601 = true;
        break;
    default:
        break;
    }
    switch (type_) {
    case type::string:
        if (s_fmt_no_quotes) {
            oss << v.s;
        } else {
            oss << '"' << v.s << '"';
        }
        break;
    case type::boolean:
        oss << v.b;
        break;
    case type::integer:
        oss << static_cast<integer>(v.u);
        break;
    case type::uinteger:
        oss << v.u;
        break;
    case type::real:
        oss << std::setprecision(9) << v.r;
        break;
    case type::timestamp:
        if (ts_fmt_iso8601) {
            /*
             * Stage the conversion of nanoseconds to a system clock
             * time point to cater for different clock precisions on
             * different host platforms. Use a temporary system clock
             * time point to add the duration to to deal with possible
             * differences in the clock types.
             */
            std::chrono::duration td = std::chrono::nanoseconds{v.t.nsecs};
            util::time::datetime_timepoint temp_dtp;
            auto dp =
                std::chrono::time_point_cast<std::chrono::system_clock::duration>(temp_dtp + td);
            oss << util::time::datetime_iso8601(dp);
        } else {
            oss << v.t.nsecs / 1000000000UL
                << '.' << std::setw(9) << std::setfill('0')
                << v.t.nsecs % 1000000000UL;
        }
        break;
    }
    if (attributes) {
        oss << " (";
        if (read_only) {
            oss << "ro";
        } else {
            oss << "rw ";
            if (write_lock) {
                oss << "locked";
            } else {
                oss << "unlocked";
            }
        }
        oss << ')';
    }
    return oss.str();
}

void node_base::set_hint(hint hint_) {
    lock_guard guard(lock);
    switch (hint_) {
    case hint::fmt_defaults:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_defaults;
        break;
    case hint::fmt_no_quotes:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_no_quotes;
        break;
    case hint::fmt_boolnum:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_boolnum;
        break;
    case hint::fmt_dec:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_dec;
        break;
    case hint::fmt_oct:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_oct;
        break;
    case hint::fmt_hex:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_hex;
        break;
    case hint::fmt_fixed:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_fixed;
        break;
    case hint::fmt_iso8601:
        hints &= ~(fmt_mask << fmt_base);
        hints |= fmt_iso8601;
        break;
    default:
        break;
    }
}

bool node_base::get_hint(hint hint_) {
    lock_guard guard(lock);
    bool result = false;
    switch (hint_) {
    case hint::fmt_defaults:
        if ((hints & fmt_defaults) != 0) {
            result = true;
        }
        break;
    case hint::fmt_no_quotes:
        if ((hints & fmt_no_quotes) != 0) {
            result = true;
        }
        break;
    case hint::fmt_boolnum:
        if ((hints & fmt_boolnum) != 0) {
            result = true;
        }
        break;
    case hint::fmt_dec:
        if ((hints & fmt_dec) != 0) {
            result = true;
        }
        break;
    case hint::fmt_oct:
        if ((hints & fmt_oct) != 0) {
            result = true;
        }
        break;
    case hint::fmt_hex:
        if ((hints & fmt_hex) != 0) {
            result = true;
        }
        break;
    case hint::fmt_fixed:
        if ((hints & fmt_fixed) != 0) {
            result = true;
        }
        break;
    case hint::fmt_iso8601:
        if ((hints & fmt_iso8601) != 0) {
            result = true;
        }
        break;
    default:
        break;
    }
    return result;
}

node& node::operator=(hint hint_) {
    set_hint(hint_);
    return *this;
}

void node::check_base() const {
    if (!valid()) {
        throw error(error::code::read_only, "mib::node::check: not a valid node");
    }
}

node::node(const name_type& name, const type type_, const bool enabled)
    : base(mib->add(name, type_, enabled)) {
}

node::node(const char* name, const type type_, const bool enabled)
    : base(mib->add(name, type_, enabled)) {
}

node::node(const name_type& name)
    : base(mib->find(name)) {
}

node::node(const char* name)
    : base(mib->find(name)) {
}

node::node() {}

node::node(const node& orig)
    : base(orig.base) {
}

node::node(node_base_ptr base_)
    : base(base_) {
}

void node::remove() {
    if (valid()) {
        mib->remove(base);
    }
}

void node::reset() {
    base.reset();
}

node& node::operator=(const node& other) {
    base = other.base;
    return *this;
}

std::string node::name() const {
    check_base();
    node_base::lock_guard guard(base->lock);
    return base->name;
}

type node::type_is() const {
    check_base();
    return base->type_;
}

bool node::read_only() const {
    check_base();
    node_base::lock_guard guard(base->lock);
    return base->read_only;
}

bool node::write_locked() const {
    check_base();
    node_base::lock_guard guard(base->lock);
    return base->read_only || base->write_lock;
}

void node::lock_writes() {
    check_base();
    node_base::lock_guard guard(base->lock);
    if (base->read_only) {
        throw error(
            error::code::read_only,
            "mib::node::lock-write: MIB is read-only: " + base->name);
    }
    base->write_lock = true;
 }

void node::unlock_writes() {
    check_base();
    node_base::lock_guard guard(base->lock);
    base->write_lock = false;
}

void node::enable() {
    check_base();
    base->enable();
}

void node::disable() {
    check_base();
    base->disable();
}

bool node::is_enabled() const {
    check_base();
    return base->is_enabled();
}

void node::set_event_func(event evt, event_func& func) {
    check_base();
    base->set_event_func(evt, func);
}

void node::clear_event_func(event evt) {
    check_base();
    base->clear_event_func(evt);
}

bool node::has_event_func(event evt) const {
    check_base();
    return base->has_event_func(evt);
}

node::operator string() {
    check_base();
    return base->get<string>();
}

node::operator boolean() {
    check_base();
    return base->get<boolean>();
}

node::operator integer() {
    check_base();
    return base->get<integer>();
}

node::operator uinteger() {
    check_base();
    return base->get<uinteger>();
}

node::operator real() {
    check_base();
    return base->get<real>();
}

node::operator timestamp() {
    check_base();
    return base->get<timestamp>();
}

void node::set_value(const std::string& val) {
    set_value(val.c_str());
}

void node::set_value(const char* val) {
    check_base();
    base->set_value(val);
}

std::string node::str(bool attributes) {
    check_base();
    return base->str(attributes);
}

void node::set_hint(hint hint_) {
    check_base();
    base->set_hint(hint_);
}

bool node::get_hint(hint hint_) const {
    check_base();
    return base->get_hint(hint_);
}

void add(const name_type& name, const type type_, const bool enabled) {
    mib->add(name, type_, enabled);
}

void add(const char* name, const type type_, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), type_, enabled);
}

void add_ro_str(const name_type& name, const string& val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_str(const char* name, const string& val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_str(const name_type& name, const stringp val, const bool enabled) {
    if (val == nullptr) {
        throw error(error::code::invalid_value, "mib::add: value empty");
    }
    mib->add(name, val, enabled);
}

void add_ro_str(const char* name, const stringp val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    if (val == nullptr) {
        throw error(error::code::invalid_value, "mib::add: value empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_bool(const name_type& name, const boolean val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_bool(const char* name, const boolean val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_int(const name_type& name, const integer val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_int(const char* name, const integer val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_uint(const name_type& name, const uinteger val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_uint(const char* name, const uinteger val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_real(const name_type& name, const real val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_real(const char* name, const real val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void add_ro_timestamp(const name_type& name, const timestamp& val, const bool enabled) {
    mib->add(name, val, enabled);
}

void add_ro_timestamp(const char* name, const timestamp& val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib->add(name_type(name), val, enabled);
}

void remove(const name_type& name) {
    node(name).remove();
}

void remove(const char* name) {
    node(name).remove();
}

node find(const name_type& name) {
    return node(name);
}

node find(const char* name) {
    return node(name);
}

void enable(const name_type& name) {
    auto n = node(name);
    n.enable();
}

void enable(const char* name) {
    enable(name_type(name));
}

void disable(const name_type& name) {
    auto n = node(name);
    n.disable();
}

void disable(const char* name) {
    disable(name_type(name));
}

bool is_enabled(const name_type& name) {
    auto n = node(name);
    return n.is_enabled();
}

bool is_enabled(const char* name) {
    return is_enabled(name_type(name));
}

bool contains(const name_type& name) {
    return mib->contains(name);
}

bool contains(const char* name) {
    if (name == nullptr) {
        return false;
    }
    return mib->contains(std::string(name));
}

void walk(mib_walk_func& walk_func) {
    mib->walk(walk_func);
}

void mib_to_json(xia::pixie::format::json& mib_json, std::string val, std::string names) {
    if (!names.empty()) {
        auto n = names.find_first_of(mib::mibsep);
        if (n == std::string::npos) {
            xia::util::string::dequote(val);
            mib_json[names] = val;
        } else {
            auto name = names.substr(0, n);
            names = names.substr(n + 1);
            mib_to_json(mib_json[name], val, names);
        }
    }
}
}  // namespace mib
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::mib::node& node) {
    (void) node;
    return out;
}
