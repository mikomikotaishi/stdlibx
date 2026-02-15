/**
 * @file functional.cppm
 * @module stdx:ranges.functional
 * @brief Module file for standard library functional ranges operations.
 *
 * This file contains the implementation of the functional ranges operations in the standard library.
 */

module;

export module stdx:ranges.functional;

import core;

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    using core::ranges::equal_to;
    using core::ranges::not_equal_to;
    using core::ranges::greater;
    using core::ranges::less;
    using core::ranges::greater_equal;
    using core::ranges::less_equal;
}
