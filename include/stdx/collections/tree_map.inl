#pragma once

using stdx::alloc::Allocator;
using stdx::meta::IsSameValue;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
        requires IsSameValue<typename Alloc::value_type, Pair<const Key, Value>>
    using TreeMap = std::map<Key, Value, Compare, Alloc>;

    template <typename Key, typename Value, typename Compare = Less<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
        requires IsSameValue<typename Alloc::value_type, Pair<const Key, Value>>
    using TreeMultimap = std::multimap<Key, Value, Compare, Alloc>;
}
