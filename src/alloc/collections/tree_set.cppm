/**
 * @file tree_set.cppm
 * @module alloc:collections.tree_set
 * @brief Module file for standard library tree set operations.
 *
 * This file contains the implementation of the tree set operations in the standard library.
 */

module;

#include <set>

export module alloc:collections.tree_set;

import core;

import :mem.memory;

using core::Less;
using core::meta::IsSameValue;
using core::meta::RemoveConstVolatile;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
        requires
            IsSameValue<typename RemoveConstVolatile<Key>::type, Key> &&
            IsSameValue<typename Alloc::value_type, Key>
    using TreeSet = std::set<Key, Compare, Alloc>;

    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
        requires
            IsSameValue<typename RemoveConstVolatile<Key>::type, Key> &&
            IsSameValue<typename Alloc::value_type, Key>
    using TreeMultiset = std::multiset<Key, Compare, Alloc>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
    using Set = TreeSet<Key, Compare, Alloc>;

    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
    using Multiset = TreeMultiset<Key, Compare, Alloc>;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename Key, typename Compare = Less<Key>>
        using TreeSet = std::pmr::set<Key, Compare>;

        template <typename Key, typename Compare = Less<Key>>
        using TreeMultiset = std::pmr::multiset<Key, Compare>;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        template <typename Key, typename Compare = Less<Key>>
        using Set = TreeSet<Key, Compare>;

        template <typename Key, typename Compare = Less<Key>>
        using Multiset = TreeMultiset<Key, Compare>;
        #endif
    }

    using std::erase_if;

    using std::operator==;
    using std::operator<=>;

    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::empty;
    using std::data;

    using std::swap;
}
