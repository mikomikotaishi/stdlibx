/**
 * @file iterator.cppm
 * @module core:ranges.iterator
 * @brief Module file for standard library iterator operations.
 *
 * This file contains the implementation of the iterator operations in the standard library.
 */

module;

#include <iterator>

export module core:ranges.iterator;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace core::ranges {
    using std::ranges::advance;
    using std::ranges::distance;
    using std::ranges::next;
    using std::ranges::prev;

    using std::ranges::iter_move;
    using std::ranges::iter_swap;
}
