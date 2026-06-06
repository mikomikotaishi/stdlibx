#pragma once

using stdx::alloc::Allocator;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using HashMap = std::unordered_map<Key, Value, HashFn, Pred, Alloc>;

    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using HashMultimap = std::unordered_multimap<Key, Value, HashFn, Pred, Alloc>;
}
