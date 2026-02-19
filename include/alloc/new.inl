#pragma once

using core::usize;

/**
 * @namespace alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace alloc {
    using AlignValue = std::align_val_t;
    using NewHandler = std::new_handler;
    using NoThrowTag = std::nothrow_t;
    using DestroyingDeleteTag = std::destroying_delete_t;
    inline constexpr NoThrowTag NoThrow = std::nothrow;
    inline constexpr DestroyingDeleteTag DestroyingDelete = std::destroying_delete;

    inline constexpr usize HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE = std::hardware_constructive_interference_size;
    inline constexpr usize HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE = std::hardware_destructive_interference_size;

    using BadAllocationException = std::bad_alloc;
    using BadArrayNewLengthException = std::bad_array_new_length;
}
