#pragma once

/**
 * @namespace stdx::ranges
 * @brief Standard library ranges operations.
 */
export namespace stdx::ranges {
    using std::ranges::advance;
    using std::ranges::distance;
    using std::ranges::next;
    using std::ranges::prev;

    using std::ranges::iter_move;
    using std::ranges::iter_swap;
}
