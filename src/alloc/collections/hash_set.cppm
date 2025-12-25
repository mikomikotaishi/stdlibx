/**
 * @file hash_set.cppm
 * @module alloc.collections.hash_set
 * @brief Module file for standard library hash set operations.
 *
 * This file contains the implementation of the hash set operations in the standard library.
 */

module;

#include <unordered_set>

export module alloc:collections.hash_set;

import core;

import :mem.memory;

using core::EqualTo;
using core::Hash;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashSet = std::unordered_set<Key, HashFn, Predicate, Alloc>;

    template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using HashMultiset = std::unordered_multiset<Key, HashFn, Predicate, Alloc>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using UnorderedSet = HashSet<Key, HashFn, Predicate, Alloc>;

    template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>, typename Alloc = Allocator<Key>>
    using UnorderedMultiset = HashMultiset<Key, HashFn, Predicate, Alloc>;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>>
        using HashSet = std::pmr::unordered_set<Key, HashFn, Predicate>;

        template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>>
        using HashMultiset = std::pmr::unordered_multiset<Key, HashFn, Predicate>;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>>
        using UnorderedSet = HashSet<Key, HashFn, Predicate>;

        template <typename Key, typename HashFn = Hash<Key>, typename Predicate = EqualTo<Key>>
        using UnorderedMultiset = HashMultiset<Key, HashFn, Predicate>;
        #endif
    }

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
