#pragma once

using core::meta::IsSameValue;

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
