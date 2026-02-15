/**
 * @file bitset.cppm
 * @module stdx:collections.bitset
 * @brief Module file for standard library bitset operations.
 *
 * This file contains the implementation of the bitset operations in the standard library.
 */

module;

export module stdx:collections.bitset;

import core;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using core::collections::BitSet;

    using core::collections::operator&;
    using core::collections::operator|;
    using core::collections::operator^;
    using core::collections::operator<<;
    using core::collections::operator>>;

    using core::collections::hash;
    using core::collections::swap;
}
