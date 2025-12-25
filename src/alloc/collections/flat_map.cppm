/**
 * @file flat_map.cppm
 * @module alloc:collections.flat_map
 * @brief Module file for standard library flat map operations.
 *
 * This file contains the implementation of the flat set operations in the standard library.
 */

module;

#if __has_include(<flat_map>)
#include <flat_map>
#endif

export module alloc:collections.flat_map;

import core;

import :collections.vector;
import :mem.memory;

using core::Less;
using core::meta::IsNothrowSwappableValue;
using core::meta::IsSameValue;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    #if __has_include(<flat_map>)
    template <typename Key, typename Value, typename Compare = Less<Key>, typename KeyContainer = Vector<Key>, typename ValueContainer = Vector<Value>>
        requires
            IsSameValue<Key, typename KeyContainer::value_type> && 
            IsSameValue<Value, typename ValueContainer::value_type> &&
            IsNothrowSwappableValue<KeyContainer> &&
            IsNothrowSwappableValue<ValueContainer>
    using FlatMap = std::flat_map<Key, Value, Compare, KeyContainer, ValueContainer>;

    template <typename Key, typename Value, typename Compare = Less<Key>, typename KeyContainer = Vector<Key>, typename ValueContainer = Vector<Value>>
        requires
            IsSameValue<Key, typename KeyContainer::value_type> && 
            IsSameValue<Value, typename ValueContainer::value_type> &&
            IsNothrowSwappableValue<KeyContainer> &&
            IsNothrowSwappableValue<ValueContainer>
    using FlatMultimap = std::flat_multimap<Key, Value, Compare, KeyContainer, ValueContainer>;

    using std::erase_if;

    using SortedUniqueTag = std::sorted_unique_t;
    inline constexpr SortedUniqueTag SortedUnique = std::sorted_unique;
    using SortedEquivalentTag = std::sorted_equivalent_t;
    inline constexpr SortedEquivalentTag SortedEquivalent = std::sorted_equivalent;
    #endif
}
