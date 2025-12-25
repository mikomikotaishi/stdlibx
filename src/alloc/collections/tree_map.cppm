/**
 * @file tree_map.cppm
 * @module alloc:collections.tree_map
 * @brief Module file for standard library tree map operations.
 *
 * This file contains the implementation of the tree map operations in the standard library.
 */

module;

#include <map>

export module alloc:collections.tree_map;

import core;

import :mem.memory;

using core::Less;
using core::Pair;
using core::meta::IsSameValue;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
        requires IsSameValue<typename Alloc::value_type, Pair<const Key, Value>>
    using TreeMap = std::map<Key, Value, Compare, Alloc>;

    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
        requires IsSameValue<typename Alloc::value_type, Pair<const Key, Value>>
    using TreeMultimap = std::multimap<Key, Value, Compare, Alloc>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using Map = TreeMap<Key, Value, Compare, Alloc>;

    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using Multimap = TreeMultimap<Key, Value, Compare, Alloc>;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename Key, typename Value, typename Compare = Less<Key>>
        using TreeMap = std::pmr::map<Key, Value, Compare>;

        template <typename Key, typename Value, typename Compare = Less<Key>>
        using TreeMultimap = std::pmr::multimap<Key, Value, Compare>;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        template <typename Key, typename Value, typename Compare = Less<Key>>
        using Map = TreeMap<Key, Value, Compare>;

        template <typename Key, typename Value, typename Compare = Less<Key>>
        using Multimap = TreeMultimap<Key, Value, Compare>;
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
