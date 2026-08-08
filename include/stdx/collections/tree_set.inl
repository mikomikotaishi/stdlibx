#pragma once

using stdx::alloc::Allocator;
using stdx::meta::RemoveConstVolatile;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
        requires
            SameAs<typename RemoveConstVolatile<Key>::type, Key> &&
            SameAs<typename Alloc::value_type, Key>
    using TreeSet = std::set<Key, Compare, Alloc>;

    template <typename Key, typename Compare = Less<Key>, typename Alloc = Allocator<Key>>
        requires
            SameAs<typename RemoveConstVolatile<Key>::type, Key> &&
            SameAs<typename Alloc::value_type, Key>
    using TreeMultiset = std::multiset<Key, Compare, Alloc>;
}
