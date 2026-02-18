#pragma once

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    using alloc::ranges::uninitialized_copy;
    using alloc::ranges::uninitialized_copy_n;
    using alloc::ranges::uninitialized_fill;
    using alloc::ranges::uninitialized_fill_n;
    using alloc::ranges::uninitialized_move;
    using alloc::ranges::uninitialized_move_n;
    using alloc::ranges::uninitialized_default_construct;
    using alloc::ranges::uninitialized_default_construct_n;
    using alloc::ranges::uninitialized_value_construct;
    using alloc::ranges::uninitialized_value_construct_n;
    using alloc::ranges::destroy;
    using alloc::ranges::destroy_n;
    using alloc::ranges::destroy_at;
    using alloc::ranges::construct_at;
}
