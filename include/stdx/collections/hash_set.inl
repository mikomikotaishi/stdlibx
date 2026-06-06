#pragma once

using stdx::alloc::Allocator;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashSet = std::unordered_set<Key, HashFn, Pred, Alloc>;

    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashMultiset = std::unordered_multiset<Key, HashFn, Pred, Alloc>;
}
