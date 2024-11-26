/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file crc.hpp
 * @brief Defines utility functions and data structures related to CRC32 useful throughout the SDK.
 */

#ifndef PIXIESDK_UTIL_CRC_HPP
#define PIXIESDK_UTIL_CRC_HPP

#include <string>
#include <vector>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace crc {

/**
 * @brief Defines a function for a CRC32 checksum
 *
 * The polynomial is
 * `X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0`
 *
 * @see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
struct crc32 {
    using value_type = uint32_t;

    value_type value;

    template<typename T>
    void update(const T val) {
        update(static_cast<const unsigned char*>(&val), sizeof(T));
    }

    template<typename T>
    void update(const std::vector<T>& vals, const size_t start = 0) {
        const size_t so = ((vals.size() - start) * sizeof(typename std::vector<T>::value_type));
        update(reinterpret_cast<const unsigned char*>(&vals[start]), int(so));
    }

    template<typename T>
    crc32& operator<<(const T& val) {
        update(val);
        return *this;
    }

    /**
     * Update the CRC with the string
     */
    crc32& operator<<(const std::string& val);

    /**
     * Update the CRC with the data in the file at the path.
     */
    void file(const std::string path);

    /**
     * Return the CRC32 value as a hex string
     */
    operator const std::string() const;

    crc32();

    void clear();

private:
    void update(const unsigned char* data, int len);
    /**
     * A precomputed lookup table for the CRC32 bitwise algorithm
     */
    static const value_type table[256];
};

} // namespace crc
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_CRC_HPP
