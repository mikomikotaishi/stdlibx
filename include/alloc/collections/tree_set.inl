#pragma once

using core::meta::IsSameValue;
using core::meta::RemoveConstVolatile;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
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
