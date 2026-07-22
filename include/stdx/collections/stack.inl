#pragma once

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename T, typename Container = Deque<T>>
        requires SameAs<T, typename Container::value_type>
    using Stack = std::stack<T, Container>;
}
