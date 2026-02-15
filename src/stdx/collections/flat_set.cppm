/**
 * @file flat_set.cppm
 * @module stdx:collections.flat_set
 * @brief Module file for standard library flat set operations.
 *
 * This file contains the implementation of the flat set operations in the standard library.
 */

module;

export module stdx:collections.flat_set;

import alloc;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #if __has_include(<flat_set>)
    using alloc::collections::FlatSet;
    using alloc::collections::FlatMultiset;

    using alloc::collections::SortedUniqueTag;
    using alloc::collections::SortedUnique;
    using alloc::collections::SortedEquivalentTag;
    using alloc::collections::SortedEquivalent;

    using alloc::collections::erase_if;
    #endif
}
