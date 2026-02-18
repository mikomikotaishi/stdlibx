#pragma once

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    using ::core::ranges::advance;
    using ::core::ranges::distance;
    using ::core::ranges::next;
    using ::core::ranges::prev;

    using ::core::ranges::iter_move;
    using ::core::ranges::iter_swap;
}
