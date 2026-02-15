/**
 * @file iterator.cppm
 * @module stdx:ranges.iterator
 * @brief Module file for standard library iterator operations.
 *
 * This file contains the implementation of the iterator operations in the standard library.
 */

module;

export module stdx:ranges.iterator;

import core;

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    using core::ranges::advance;
    using core::ranges::distance;
    using core::ranges::next;
    using core::ranges::prev;

    using core::ranges::iter_move;
    using core::ranges::iter_swap;
}
