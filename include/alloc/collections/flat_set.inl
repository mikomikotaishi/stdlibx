#pragma once

using core::Less;
using core::meta::IsNothrowSwappableValue;
using core::meta::IsSameValue;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename Key, typename Compare = Less<Key>, typename Container = Vector<Key>>
        requires
            IsSameValue<Key, typename Container::value_type> &&
            IsNothrowSwappableValue<Container>
    using FlatSet = std::flat_set<Key, Compare, Container>;

    template <typename Key, typename Compare = Less<Key>, typename Container = Vector<Key>>
        requires
            IsSameValue<Key, typename Container::value_type> &&
            IsNothrowSwappableValue<Container>
    using FlatMultiset = std::flat_multiset<Key, Compare, Container>;

    using std::erase_if;

    using SortedUniqueTag = std::sorted_unique_t;
    inline constexpr SortedUniqueTag SortedUnique = std::sorted_unique;
    using SortedEquivalentTag = std::sorted_equivalent_t;
    inline constexpr SortedEquivalentTag SortedEquivalent = std::sorted_equivalent;
}
