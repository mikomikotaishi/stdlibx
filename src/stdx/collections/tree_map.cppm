/**
 * @file tree_map.cppm
 * @module stdx:collections.tree_map
 * @brief Module file for standard library tree map operations.
 *
 * This file contains the implementation of the tree map operations in the standard library.
 */

module;

export module stdx:collections.tree_map;

import alloc;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using alloc::collections::TreeMap;
    using alloc::collections::TreeMultimap;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::TreeMap;
        using alloc::collections::pmr::TreeMultimap;
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

    using alloc::collections::swap;
}
