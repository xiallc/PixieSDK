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

/** @file mib.hpp
 * @brief Management Information Base (MUB).
 */

#ifndef PIXIESDK_MIB_H
#define PIXIESDK_MIB_H

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include <pixie/error.hpp>

namespace xia {
namespace mib {
using error = xia::pixie::error::error;

/**
 * MIB separator
 */
static constexpr char mibsep = '.';

/**
 * Name of a MIB, always absolute
 */
using name_type = std::string;

/**
 * User accessable MIB node
 */
struct node;

/**
 * The types of MIB nodes
 */
enum struct type {
    string,         /**< ASCII nul terminated string */
    boolean,        /**< True or false logic value */
    integer,        /**< Signed integer at least 64 bits in size*/
    uinteger,       /**< Unsigned integer at least 64 bits in size  */
    real,           /**< Real number */
    timestamp       /**< Time stamp in nanoseconds */
};

/**
 * The type of the types for a MIB
 */
using string = std::string;
using stringp = const char*;
using boolean = bool;
using integer = int64_t;
using uinteger = uint64_t;
using real = double;
struct timestamp {
    uint64_t nsecs;
    timestamp(const uint64_t ns) : nsecs(ns) {}
    timestamp(const xia::mib::timestamp& other) : nsecs(other.nsecs) {}
    timestamp& operator=(const timestamp& other) {
        nsecs = other.nsecs;
        return *this;
    }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_integral_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    timestamp& operator=(const T val) {
        nsecs = static_cast<uint64_t>(val);
        return *this;
    }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_integral_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    operator T() { return static_cast<T>(nsecs); }
};

/**
 * Helper trait to detect if the type os atomic
 */
template<typename T> auto constexpr is_atomic = false;
template<typename T> auto constexpr is_atomic<std::atomic<T>> = true;

/**
 * Get the type from the typename
 */
template<typename T> type get_type_t() {
    if (std::is_same_v<T, string> ||
        std::is_same_v<T, stringp> ||
        std::is_convertible_v<T, stringp>) {
        return type::string;
    } else if (std::is_same_v<T, boolean>) {
        return type::boolean;
    } else if (!std::is_same_v<T, boolean> &&
               std::is_integral_v<T> &&
               std::is_arithmetic_v<T>) {
        if (std::is_signed_v<T> &&
            std::is_convertible_v<T, integer>) {
            return type::integer;
        } else if (std::is_unsigned_v<T> &&
                   std::is_convertible_v<T, uinteger>) {
            return type::uinteger;
        }
    } else if (std::is_same_v<T, real> ||
               std::is_floating_point_v<T>) {
        return type::real;
    } else if (std::is_same_v<T, timestamp>) {
        return type::timestamp;
    }
    std::string s("mib::type: invalid MIB type: ");
    throw error(error::code::invalid_value, s + typeid(T).name());
}

/*
 * GCC does not define valur_type in atomics until somewhere between
 * version 9 and version 11 of the compiler. Hack around the issue.
 *
 * On gcc that support value_type any atomic type is supported while
 * on gcc 8 only internal and double are suppored.
 */
#ifdef __GNUC__
#  include <features.h>
#  if __GNUC_PREREQ(10,0)
#define ATOMIC_VALUE_TYPE_SUPPORTED 1
#define T_value_type T::value_type
#  else
#define ATOMIC_VALUE_TYPE_SUPPORTED 0
#define T_value_type T::__integral_type
#  endif
#else
#define ATOMIC_VALUE_TYPE_SUPPORTED 1
#define T_value_type T::value_type
#endif

template<typename T,
         std::enable_if_t<
             is_atomic<T> &&
             !std::is_same_v<T, std::atomic<double>>, bool> = true>
type get_type() {
    return get_type_t<typename T_value_type>();
}
template<typename T,
         std::enable_if_t<std::is_same_v<T, std::atomic<double>>, bool> = true>
type get_type() {
    return get_type_t<real>();
}
template<typename T, std::enable_if_t<!is_atomic<T>, bool> = true>
type get_type() {
    return get_type_t<T>();
}

/**
 * MIB node data
 *
 * Integer data is stored as unsigned.
 */
union data_type {
    string s;
    boolean b;
    uinteger u;
    real r;
    timestamp t;

    data_type();
    ~data_type();

    /*
     * Use traits to select the approriate function.
     */

    /*
     * Set
     */
    template<
        typename T, std::enable_if_t<std::is_same_v<T, string> ||
                                     std::is_same_v<T, stringp> ||
                                     std::is_convertible_v<T, stringp>, bool> = true>
    void set(const T& val) { s = val; }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, boolean>, bool> = true>
    void set(const T& val) { b = val; }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_integral_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    void set(const T& val) { u = static_cast<uinteger>(val); }
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         !std::is_same_v<U, boolean> &&
                         std::is_integral_v<U> &&
                         std::is_arithmetic_v<U>, bool> = true>
    void set(const T& val) { u = static_cast<uinteger>(val.load()); }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, real> ||
                                     std::is_floating_point_v<T>, bool> = true>
    void set(const T& val) { r = val; }
    #if ATOMIC_VALUE_TYPE_SUPPORTED
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<U, real> ||
                          std::is_floating_point_v<U>), bool> = true>
    void set(const T& val) { r = val.load(); }
    #else
    template<
        typename T,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<T, std::atomic<real>> ||
                          std::is_floating_point_v<T>), bool> = true>
    void set(const T& val) { r = val.load(); }
    #endif
    template<
        typename T, std::enable_if_t<std::is_same_v<T, timestamp>, bool> = true>
    void set(const T& val) { t = val; }

    /*
     * Get
     */
    template<
        typename T, std::enable_if_t<std::is_same_v<T, string> ||
                                     std::is_same_v<T, stringp>, bool> = true>
    T get() const { return s; }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, boolean>, bool> = true>
    T get() const { return b; }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_integral_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    T get() const { return static_cast<T>(u); }
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         !std::is_same_v<U, boolean> &&
                         std::is_integral_v<U> &&
                         std::is_arithmetic_v<U>, bool> = true>
    U get() const { return static_cast<U>(u); }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, real> ||
                                     std::is_floating_point_v<T>, bool> = true>
    T get() const { return r; }
    #if ATOMIC_VALUE_TYPE_SUPPORTED
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<U, real> ||
                          std::is_floating_point_v<U>), bool> = true>
    U get() const { return r; }
    #else
    template<
        typename T,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<T, std::atomic<real>> ||
                          std::is_floating_point_v<T>), bool> = true>
    real get() const { return r; }
    #endif
    template<
        typename T, std::enable_if_t<std::is_same_v<T, timestamp>, bool> = true>
    T get() const { return t; }

    /*
     * Compare
     */
    template<
        typename T, std::enable_if_t<std::is_same_v<T, string> ||
                                     std::is_same_v<T, stringp> ||
                                     std::is_convertible_v<T, stringp>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return s < val; }
        if (op == '>') { return s > val; }
        return s == val;
    }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, boolean>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return b < val; }
        if (op == '>') { return b > val; }
        return b == val;
    }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_integral_v<T> &&
                                     std::is_signed_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    bool cmp(const T& val, const char op) const {
        integer i = static_cast<integer>(u);
        if (op == '<') { return i < val; }
        if (op == '>') { return i > val; }
        return i == val;
    }
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         !std::is_same_v<U, boolean> &&
                         std::is_integral_v<U> &&
                         std::is_signed_v<U> &&
                         std::is_arithmetic_v<U>, bool> = true>
    bool cmp(const T& val, const char op) const {
        integer i = static_cast<integer>(u);
        if (op == '<') { return i < val.load(); }
        if (op == '>') { return i > val.load(); }
        return i == val.load();
    }
    template<
        typename T, std::enable_if_t<!std::is_same_v<T, boolean> &&
                                     std::is_same_v<T, T> &&
                                     std::is_unsigned_v<T> &&
                                     std::is_arithmetic_v<T>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return u < val; }
        if (op == '>') { return u > val; }
        return u == val;
    }
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         !std::is_same_v<U, boolean> &&
                         std::is_integral_v<U> &&
                         std::is_unsigned_v<U> &&
                         std::is_arithmetic_v<U>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return u < val.load(); }
        if (op == '>') { return u > val.load(); }
        return u == val.load();
    }
    template<
        typename T, std::enable_if_t<std::is_same_v<T, real> ||
                                     std::is_floating_point_v<T>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return r < val; }
        if (op == '>') { return r > val; }
        return r == val;
    }
    #if ATOMIC_VALUE_TYPE_SUPPORTED
    template<
        typename T, typename U = typename T_value_type,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<U, real> ||
                          std::is_floating_point_v<U>), bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return r < val.load(); }
        if (op == '>') { return r > val.load(); }
        return r == val.load();
    }
    #else
    template<
        typename T,
        std::enable_if_t<is_atomic<T> &&
                         (std::is_same_v<T, std::atomic<real>> ||
                          std::is_floating_point_v<T>), bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return r < val.load(); }
        if (op == '>') { return r > val.load(); }
        return r == val.load();
    }
    #endif
    template<
        typename T, std::enable_if_t<std::is_same_v<T, timestamp>, bool> = true>
    bool cmp(const T& val, const char op) const {
        if (op == '<') { return t < val; }
        if (op == '>') { return t > val; }
        return t == val;
    }
};

/**
 * Events when a node is written to or read. The node is not locked
 * when the evetn function is called.
 */
enum struct event {
    set,   /**< Called after the value is set */
    get,   /**< Called before the value is read */
    timer  /**< Called when the MIB timer expires */
};

/**
 * Called when an event happens on a MIB. Not supported for read-only
 * MIBs.
 *
 * Capture any references for specific instances in a lambda.
 *
 * @warning There are no type checks when accessing the data
 *          directly. It is assumed you know the type in event handler
 *          and you do not set the wrong entry. Results are
 *          unpredicable if the wrong union element is accessed.
 */
using event_func = std::function<void(event evt, type type_, data_type& data)>;

/**
 * Guard event calls so any calls to set and get from the event do not
 * result in the event call being taken again. The node is not locked
 * when in an event call but locked when not.
 */
template<class M> struct event_call_guard {
    M& m;
    bool& b;
    event_call_guard(M& m_, bool& b_)
        : m(m_), b(b_) {
        b = true;
        m.unlock();
    }
    ~event_call_guard() {
        m.lock();
        b = false;
    }
};

/**
 * A node base lives in the MIB tree. A node references the node base
 * and a node_base can be shared.
 *
 * Integers are stored as unsigned integers and the type::integer is
 * only valid when formatting the data. This lets the assignment and
 * casting operators work using the compiler's implicit rules.
 *
 * Becareful with the union as it contains classes which complicate
 * it.
 */
struct node_base {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;

    const name_type name;
    const type type_;
    const bool read_only;
    bool write_lock;
    bool enabled;
    data_type v;
    event_func get_event;
    event_func set_event;
    event_func timer_event;
    bool in_event_call;
    lock_type lock;

    node_base(const name_type& name, const type type_, const bool enabled);
    /*
     * Read only MIBs
     */
    template<typename T> node_base(
        const name_type& name, const T& val, const bool enabled);
    ~node_base();

    bool operator==(const node_base& other) const;
    bool operator<(const node_base& other) const;

    void enable();
    void disable();
    bool is_enabled();

    template<typename T> void set(const T& val);
    template<typename T> T get();
    template<typename T> bool cmp(const T& val, const char op);

    void check(const type type_);

    void set_event_func(event evt, event_func& func);
    void clear_event_func(event evt);
    bool has_event_func(event evt);

    void call_set_event_func();
    void call_get_event_func();
    void call_timer_event_func();

    std::string str(bool attributes);
};

/**
 * Node base pointers need to reference counted.
 */
using node_base_ptr = std::shared_ptr<node_base>;

/**
 * A node in MIB tree. A node as a single type for it's life time.
 */
struct node {
    /**
     * Add a MIB node
     */
    node(const name_type& name, const type type_, const bool enabled = true);
    node(const char* name, const type type_, const bool enabled = true);
    /**
     * Access a MIB node
     */
    node(const name_type& name);
    node(const char* name);
    /**
     * Empty
     */
    node();
    /**
     * Copy
     */
    node(const node& node);
    /**
     * Internal
     */
    node(node_base_ptr base);

    /**
     * Remove the MIB deleting the entry. This makes the node invalid.
     */
    void remove();

    /**
     * Reset the node making it invalid
     */
    void reset();

    node& operator=(const node& other);

    std::string name() const;
    type type_is() const;
    bool read_only() const;

    void enable();
    void disable();
    bool is_enabled() const;

    bool write_locked() const;
    void lock_writes();
    void unlock_writes();

    void set_event_func(event evt, event_func& func);
    void clear_event_func(event evt);
    bool has_event_func(event evt) const;

    template<typename T> node& operator=(const T& val);
    template<typename T> bool operator==(const T& val) const;
    template<typename T> bool operator!=(const T& val) const;
    template<typename T> bool operator<(const T& val) const;
    template<typename T> bool operator>(const T& val) const;
    template<typename T> bool operator<=(const T& val) const;
    template<typename T> bool operator>=(const T& val) const;

    operator string();
    operator boolean();
    operator integer();
    operator uinteger();
    operator real();
    operator timestamp();

    template<typename T> void set(const T& val);
    template<typename T> T get() const;

    std::string str(bool attributes = false);

    bool valid() const { if (base) return true; return false; }

private:
    void check_base() const;
    node_base_ptr base;
};

/**
 * Read/write references to data. You need to handle locking
 */
enum struct rw_mode { ro, wo, rw };
template<typename T> struct read_write {
    rw_mode mode;
    node nod;
    T& val;
    read_write(const name_type& name, T& val_, rw_mode mode_ = rw_mode::ro)
        : mode(mode_), nod(name, get_type<T>()), val(val_) {
        event_func handler =
            [&self = *this](mib::event evt, mib::type , data_type& data) {
                if (evt == event::get) {
                    data.set<T>(self.val);
                } else {
                    self.val = data.get<T>();
                }
            };
        if (mode == rw_mode::ro) {
            nod.lock_writes();
        }
        if (mode == rw_mode::wo || mode == rw_mode::rw) {
            nod.set_event_func(event::set, handler);
        }
        if (mode == rw_mode::ro || mode == rw_mode::rw) {
            nod.set_event_func(event::get, handler);
        }
    }
    read_write() = delete;
    ~read_write() {
        nod.clear_event_func(event::set);
        nod.clear_event_func(event::get);
    }
    node& operator*() { return nod; }
    node* operator->() { return &nod; }
};

/**
 * Read/write references to data. Lockind is via the lock you provide.
 */
template<typename T, typename M> struct read_write_lock {
    rw_mode mode;
    node nod;
    T& val;
    M& lock;
    read_write_lock(
        const name_type& name, T& val_, M& lock_, rw_mode mode_ = rw_mode::ro)
        : mode(mode_), nod(name, get_type<T>()), val(val_), lock(lock_) {
        event_func handler =
            [&self = *this](mib::event evt, mib::type , data_type& data) {
                std::lock_guard<M> guard(self.lock);
                if (evt == event::get) {
                    data.set<T>(self.val);
                } else {
                    self.val = data.get<T>();
                }
            };
        if (mode == rw_mode::ro) {
            nod.lock_writes();
        }
        if (mode == rw_mode::wo || mode == rw_mode::rw) {
            nod.set_event_func(event::set, handler);
        }
        if (mode == rw_mode::ro || mode == rw_mode::rw) {
            nod.set_event_func(event::get, handler);
        }
    }
    ~read_write_lock() {
        nod.clear_event_func(event::set);
        nod.clear_event_func(event::get);
    }
    node& operator*() { return nod; }
    node* operator->() { return &nod; }
};

/**
 * Containers for common types
 */
using nodes = std::vector<node>;
using read_write_int_nodes = std::vector<read_write<int>>;
using read_write_uint_nodes = std::vector<read_write<unsigned int>>;
using read_write_size_t_nodes = std::vector<read_write<size_t>>;
using read_write_double_nodes = std::vector<read_write<double>>;

template<typename M>
using read_write_lock_int_nodes = std::vector<read_write_lock<int, M>>;
template<typename M>
using read_write_lock_uint_nodes = std::vector<read_write_lock<unsigned int, M>>;
template<typename M>
using read_write_lock_size_t_nodes = std::vector<read_write_lock<size_t, M>>;
template<typename M>
using read_write_lock_double_nodes = std::vector<read_write_lock<double, M>>;


template<typename T> node_base::node_base(
    const name_type& name_, const T& val, const bool enabled_)
    : name(name_), type_(get_type<T>()), read_only(true), write_lock(false),
      enabled(enabled_), in_event_call(false) {
    v.set<T>(val);
}

template<typename T> void node_base::set(const T& val) {
    lock_guard guard(lock);
    if (read_only) {
        throw error(error::code::read_only, "mib::node::set: MIB is read-only");
    }
    if (write_lock) {
        throw error(error::code::read_only, "mib::node::set: MIB is write locked");
    }
    if (std::is_same_v<T, string> ||
        std::is_same_v<T, stringp> ||
        std::is_convertible_v<T, stringp>) {
        check(type::string);
    } else if (std::is_same_v<T, boolean>) {
        check(type::boolean);
    } else if (!std::is_same_v<T, boolean> &&
               std::is_integral_v<T> &&
               std::is_arithmetic_v<T>) {
        if (type_ != type::integer && type_ != type::uinteger) {
            if (std::is_signed_v<T>) {
                check(type::integer);
            }
            check(type::uinteger);
        }
    } else if (std::is_same_v<T, real> ||
               std::is_floating_point_v<T>) {
        check(type::real);
    } else if (std::is_same_v<T, timestamp>) {
        check(type::timestamp);
    } else {
        std::string s("mib::node::set: invalid MIB type: ");
        throw error(error::code::invalid_value, s + typeid(T).name());
    }
    v.set<T>(val);
    call_set_event_func();
}

template<typename T> T node_base::get() {
    lock_guard guard(lock);
    if (std::is_class_v<T> ||
        std::is_same_v<T, stringp> ||
        std::is_convertible_v<T, stringp>) {
        check(type::string);
    } else if (std::is_same_v<T, boolean>) {
        check(type::boolean);
    } else if (!std::is_same_v<T, boolean> &&
               std::is_integral_v<T> &&
               std::is_arithmetic_v<T>) {
        if (type_ != type::integer && type_ != type::uinteger) {
            if (std::is_signed_v<T>) {
                check(type::integer);
            }
            check(type::uinteger);
        }
    } else if (std::is_same_v<T, real> ||
               std::is_floating_point_v<T>) {
        check(type::real);
    } else if (std::is_same_v<T, timestamp>) {
        check(type::timestamp);
    } else {
        std::string s("mib::node::get: invalid MIB type: ");
        throw error(error::code::invalid_value, s + typeid(T).name());
    }
    call_get_event_func();
    return v.get<T>();
}

template<typename T> bool node_base::cmp(const T& val, const char op) {
    lock_guard guard(lock);
    if (std::is_same_v<T, string> ||
        std::is_same_v<T, stringp> ||
        std::is_convertible_v<T, stringp>) {
        check(type::string);
    } else if (std::is_same_v<T, boolean>) {
        check(type::boolean);
    } else if (!std::is_same_v<T, boolean> &&
               std::is_integral_v<T> &&
               std::is_arithmetic_v<T>) {
        if (type_ != type::integer && type_ != type::uinteger) {
            check(type::integer);
        }
    } else if (std::is_same_v<T, real> ||
               std::is_floating_point_v<T>) {
        check(type::real);
    } else if (std::is_same_v<T, timestamp>) {
        check(type::timestamp);
    } else {
        std::string s("mib::node::cmp: invalid MIB type: ");
        throw error(error::code::invalid_value, s + typeid(T).name());
    }
    call_get_event_func();
    return v.cmp<T>(val, op);
}

template<typename T> void node::set(const T& val) {
    check_base();
    base->set(val);
}

template<typename T> T node::get() const {
    check_base();
    return base->get<T>();
}

template<typename T> node& node::operator=(const T& val) {
    check_base();
    base->set(val);
    return *this;
}

template<typename T> bool node::operator==(const T& val) const {
    check_base();
    return base->cmp(val, '=');
}

template<typename T> bool node::operator!=(const T& val) const {
    check_base();
    return !base->cmp(val, '=');
}

template<typename T> bool node::operator<(const T& val) const {
    check_base();
    return base->cmp(val, '<');
}

template<typename T> bool node::operator>(const T& val) const {
    check_base();
    return base->cmp(val, '>');
}

template<typename T> bool node::operator<=(const T& val) const {
    check_base();
    return !base->cmp(val, '>');
}

template<typename T> bool node::operator>=(const T& val) const {
    check_base();
    return !base->cmp(val, '<');
}

/**
 * Add a MIB
 */
void add(const name_type& name, const type type_, const bool enabled = true);
void add(const char* name, const type type_, const bool enabled = true);

/**
 * Remove a MIB
 */
void remove(const name_type& name);
void remove(const char* name);

/**
 * Find a MIB
 */
node find(const name_type& name);
node find(const char* name);

/**
 * Add a read-only MIB
 */
void add_ro_str(const name_type& name, const string& val, const bool enabled = true);
void add_ro_str(const char* name, const string& val, const bool enabled = true);
void add_ro_str(const name_type& name, const stringp val, const bool enabled = true);
void add_ro_str(const char* name, const stringp val, const bool enabled = true);
void add_ro_bool(const name_type& name, const boolean val, const bool enabled = true);
void add_ro_bool(const char* name, const boolean val, const bool enabled = true);
void add_ro_int(const name_type& name, const integer val, const bool enabled = true);
void add_ro_int(const char* name, const integer val, const bool enabled = true);
void add_ro_uint(const name_type& name, const uinteger val, const bool enabled = true);
void add_ro_uint(const char* name, const uinteger val, const bool enabled = true);
void add_ro_real(const name_type& name, const real val, const bool enabled = true);
void add_ro_real(const char* name, const real val, const bool enabled = true);
void add_ro_timestamp(const name_type& name, const timestamp& val, const bool enabled = true);
void add_ro_timestamp(const char* name, const timestamp& val, const bool enabled = true);

/**
 * Control
 */
void enable(const name_type& name);
void enable(const char* name);
void disable(const name_type& name);
void disable(const char* name);
bool is_enabled(const name_type& name);
bool is_enabled(const char* name);

/**
 * See if the MIB exists?
 */
bool contains(const name_type& name);
bool contains(const char* name);

/**
 * Walk the MIB
 */
using mib_walk_func = std::function<void(node& nod)>;
void walk(mib_walk_func& walk_func);

} // mib
} // xia
#endif // PIXIESDK_MIB_H
