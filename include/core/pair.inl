#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T1, typename T2>
    using Pair = std::pair<T1, T2>;

    using std::swap;
    using std::get;

    using std::operator==;
    using std::operator<=>;
}
