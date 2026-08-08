#pragma once

using stdx::meta::IsNothrowSwappableValue;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_flat_set
    template <typename Key, typename Compare = Less<Key>, typename Container = Vector<Key>>
        requires
            SameAs<Key, typename Container::value_type> &&
            IsNothrowSwappableValue<Container>
    using FlatSet = std::flat_set<Key, Compare, Container>;

    template <typename Key, typename Compare = Less<Key>, typename Container = Vector<Key>>
        requires
            SameAs<Key, typename Container::value_type> &&
            IsNothrowSwappableValue<Container>
    using FlatMultiset = std::flat_multiset<Key, Compare, Container>;
    #endif
}
