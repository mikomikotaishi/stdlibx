#pragma once

using stdx::alloc::Allocator;
using stdx::meta::IsSameValue;
using stdx::meta::RemoveConstVolatile;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
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
}
