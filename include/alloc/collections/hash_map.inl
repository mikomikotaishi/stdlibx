#pragma once

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using HashMap = std::unordered_map<Key, Value, HashFn, Predicate, Alloc>;

    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Pair<const Key, Value>>>
    using HashMultimap = std::unordered_multimap<Key, Value, HashFn, Predicate, Alloc>;

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

    using std::hash;
    using std::swap;
}
