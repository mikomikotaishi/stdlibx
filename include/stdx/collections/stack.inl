#pragma once

using stdx::meta::IsSameValue;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename T, typename Container = Deque<T>>
        requires IsSameValue<T, typename Container::value_type>
    using Stack = std::stack<T, Container>;
}
