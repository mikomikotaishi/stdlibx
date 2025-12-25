/**
 * @file memory.cppm
 * @module alloc:ranges.memory
 * @brief Module file for standard library memory ranges operations.
 *
 * This file contains the implementation of the memory ranges operations in the standard library.
 */

module;

#include <memory>

export module alloc:ranges.memory;

/**
 * @namespace alloc::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace alloc::ranges {
    using std::ranges::uninitialized_copy;
    using std::ranges::uninitialized_copy_n;
    using std::ranges::uninitialized_fill;
    using std::ranges::uninitialized_fill_n;
    using std::ranges::uninitialized_move;
    using std::ranges::uninitialized_move_n;
    using std::ranges::uninitialized_default_construct;
    using std::ranges::uninitialized_default_construct_n;
    using std::ranges::uninitialized_value_construct;
    using std::ranges::uninitialized_value_construct_n;
    using std::ranges::destroy;
    using std::ranges::destroy_n;
    using std::ranges::destroy_at;
    using std::ranges::construct_at;
}
