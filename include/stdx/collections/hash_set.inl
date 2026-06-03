#pragma once

using stdx::alloc::Allocator;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashSet = std::unordered_set<Key, HashFn, Pred, Alloc>;

    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashMultiset = std::unordered_multiset<Key, HashFn, Pred, Alloc>;

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
