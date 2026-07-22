#pragma once

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename T, typename Container = Deque<T>>
        requires SameAs<T, typename Container::value_type>
    using Queue = std::queue<T, Container>;

    template <typename T, typename Container = Vector<T>, typename Compare = Less<typename Container::value_type>>
        requires SameAs<T, typename Container::value_type>
    using PriorityQueue = std::priority_queue<T, Container, Compare>;
}
