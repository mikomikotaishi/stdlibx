#pragma once

/**
 * @namespace stdx::alloc
 * @brief Standard library allocation operations.
 */
export namespace stdx::alloc {
    using AlignValue = std::align_val_t;
    using NewHandler = std::new_handler;

    using BadAllocationException = std::bad_alloc;
    using BadArrayLengthException = std::bad_array_new_length;
}
