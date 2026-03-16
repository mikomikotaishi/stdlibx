#pragma once

/**
 * @namespace core::prelude
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core::prelude {
    template <typename T1, typename T2>
    using Pair = std::pair<T1, T2>;

    using std::make_pair;

    using std::swap;
    using std::get;

    using std::operator==;
    using std::operator<=>;
}
