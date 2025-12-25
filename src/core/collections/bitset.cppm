/**
 * @file bitset.cppm
 * @module core:collections.bitset
 * @brief Module file for standard library bitset operations.
 *
 * This file contains the implementation of the bitset operations in the standard library.
 */

module;

#include <bitset>

export module core:collections.bitset;

/**
 * @namespace core::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace core::collections {
    template <std::size_t N>
    using BitSet = std::bitset<N>;

    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator<<;
    using std::operator>>;

    using std::hash;
    using std::swap;
}
