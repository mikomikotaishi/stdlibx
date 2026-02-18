#pragma once

using core::Less;
using core::meta::IsSameValue;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename T, typename Container = Deque<T>>
        requires IsSameValue<T, typename Container::value_type>
    using Queue = std::queue<T, Container>;

    template <typename T, typename Container = Vector<T>, typename Compare = Less<typename Container::value_type>>
        requires IsSameValue<T, typename Container::value_type>
    using PriorityQueue = std::priority_queue<T, Container, Compare>;

    using std::erase;
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
    using std::formatter;
    using std::swap;
}
