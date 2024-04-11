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

namespace xia {
namespace mib {

/**
 * The MIB is held in a single map with each MIB node holding its
 * complete path. With the low number of nodes and the size of the
 * full paths it is simpler than adding complexity to hold the MIB as
 * a tree structure.
 */
struct mib_nodes {
    struct name_cmp {
        bool operator()(const name_type& lhs, const name_type& rhs) const {
            util::string::strings lhss;
            util::string::strings rhss;
            util::string::split(lhss, lhs, mib::mibsep);
            util::string::split(rhss, rhs, mib::mibsep);
            auto pieces = std::min(lhss.size(), rhss.size());
            for (size_t p = 0; p < pieces; ++p) {
                if (lhss[p] != rhss[p]) {
                    bool l_isnum = util::string::check_number(lhss[p]);
                    bool r_isnum = util::string::check_number(rhss[p]);
                    if (!l_isnum && !r_isnum) {
                        return lhss[p] < rhss[p];
                    } else if (l_isnum && r_isnum) {
                        return std::stoi(lhss[p]) < std::stoi(rhss[p]);
                    } else if (l_isnum) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
            return lhss.size() < rhss.size();
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

mib_nodes mib;

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
    return mib.add(name_type(name), type_, enabled);
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
    for (auto& [key, base] : mib.nodes) {
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
      enabled(enabled_), in_event_call(false) {
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
    std::ostringstream oss;
    lock_guard guard(lock);
    call_get_event_func();
    switch (type_) {
    case type::string:
        oss << '"' << v.s << '"';
        break;
    case type::boolean:
        oss << std::boolalpha << v.b;
        break;
    case type::integer:
        oss << static_cast<integer>(v.u);
        break;
    case type::uinteger:
        oss << v.u;
        break;
    case type::real:
        oss << std::fixed << std::setprecision(9) << v.r;
        break;
    case type::timestamp:
        oss << v.t.nsecs / 1000000000UL
            << '.' << std::setw(9) << std::setfill('0')
            << v.t.nsecs % 1000000000UL;
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

void node::check_base() const {
    if (!valid()) {
        throw error(error::code::read_only, "mib::node::check: not a valid node");
    }
}

node::node(const name_type& name, const type type_, const bool enabled)
    : base(mib.add(name, type_, enabled)) {
}

node::node(const char* name, const type type_, const bool enabled)
    : base(mib.add(name, type_, enabled)) {
}

node::node(const name_type& name)
    : base(mib.find(name)) {
}

node::node(const char* name)
    : base(mib.find(name)) {
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
        mib.remove(base);
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
            error::code::read_only, "mib::node::lock-write: MIB is read-only");
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

void add(const name_type& name, const type type_, const bool enabled) {
    mib.add(name, type_, enabled);
}

void add(const char* name, const type type_, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mxpib::add: name empty");
    }
    mib.add(name_type(name), type_, enabled);
}

void add_ro_str(const name_type& name, const string& val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_str(const char* name, const string& val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_str(const name_type& name, const stringp val, const bool enabled) {
    if (val == nullptr) {
        throw error(error::code::invalid_value, "mib::add: value empty");
    }
    mib.add(name, val, enabled);
}

void add_ro_str(const char* name, const stringp val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    if (val == nullptr) {
        throw error(error::code::invalid_value, "mib::add: value empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_bool(const name_type& name, const boolean val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_bool(const char* name, const boolean val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_int(const name_type& name, const integer val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_int(const char* name, const integer val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_uint(const name_type& name, const uinteger val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_uint(const char* name, const uinteger val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_real(const name_type& name, const real val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_real(const char* name, const real val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
}

void add_ro_timestamp(const name_type& name, const timestamp& val, const bool enabled) {
    mib.add(name, val, enabled);
}

void add_ro_timestamp(const char* name, const timestamp& val, const bool enabled) {
    if (name == nullptr) {
        throw error(error::code::invalid_value, "mib::add: name empty");
    }
    mib.add(name_type(name), val, enabled);
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
    return mib.contains(name);
}

bool contains(const char* name) {
    if (name == nullptr) {
        return false;
    }
    return mib.contains(std::string(name));
}

void walk(mib_walk_func& walk_func) {
    mib.walk(walk_func);
}
}  // namespace mib
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::mib::node& node) {
    (void) node;
    return out;
}
