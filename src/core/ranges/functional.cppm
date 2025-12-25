/**
 * @file functional.cppm
 * @module core:ranges.functional
 * @brief Module file for standard library functional ranges operations.
 *
 * This file contains the implementation of the functional ranges operations in the standard library.
 */

module;

#include <functional>

export module core:ranges.functional;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace core::ranges {
    using std::ranges::equal_to;
    using std::ranges::not_equal_to;
    using std::ranges::greater;
    using std::ranges::less;
    using std::ranges::greater_equal;
    using std::ranges::less_equal;
}
