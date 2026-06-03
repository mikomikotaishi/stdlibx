#pragma once

/**
 * @namespace stdx::pmr::collections
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::collections {
    template <typename T>
    using Deque = std::pmr::deque<T>;

    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>>
    using HashMap = std::pmr::unordered_map<Key, Value, HashFn, Pred>;

    template <typename Key, typename Value, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>>
    using HashMultimap = std::pmr::unordered_multimap<Key, Value, HashFn, Pred>;

    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>>
    using HashSet = std::pmr::unordered_set<Key, HashFn, Pred>;

    template <typename Key, typename HashFn = Hash<Key>, typename Pred = EqualTo<Key>>
    using HashMultiset = std::pmr::unordered_multiset<Key, HashFn, Pred>;

    template <typename T>
    using LinkedList = std::pmr::list<T>;

    template <typename T>
    using SinglyLinkedList = std::pmr::forward_list<T>;

    template <typename Key, typename Value, typename Compare = Less<Key>>
    using TreeMap = std::pmr::map<Key, Value, Compare>;

    template <typename Key, typename Value, typename Compare = Less<Key>>
    using TreeMultimap = std::pmr::multimap<Key, Value, Compare>;

    template <typename Key, typename Compare = Less<Key>>
    using TreeSet = std::pmr::set<Key, Compare>;

    template <typename Key, typename Compare = Less<Key>>
    using TreeMultiset = std::pmr::multiset<Key, Compare>;

    template <typename T>
    using Vector = std::pmr::vector<T>;
}
