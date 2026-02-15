/**
 * @file hash_set.cppm
 * @module stdx:collections.hash_set
 * @brief Module file for standard library hash set operations.
 *
 * This file contains the implementation of the hash set operations in the standard library.
 */

module;

export module stdx:collections.hash_set;

import alloc;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using alloc::collections::HashSet;
    using alloc::collections::HashMultiset;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::HashSet;
        using alloc::collections::pmr::HashMultiset;
    }

    using alloc::collections::erase_if;

    using alloc::collections::operator==;
    using alloc::collections::operator<=>;

    using alloc::collections::begin;
    using alloc::collections::cbegin;
    using alloc::collections::end;
    using alloc::collections::cend;
    using alloc::collections::rbegin;
    using alloc::collections::crbegin;
    using alloc::collections::rend;
    using alloc::collections::crend;
    using alloc::collections::size;
    using alloc::collections::ssize;
    using alloc::collections::empty;
    using alloc::collections::data;

    using alloc::collections::hash;
    using alloc::collections::swap;
}
