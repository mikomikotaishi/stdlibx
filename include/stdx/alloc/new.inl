#pragma once

/**
 * @namespace stdx::alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace stdx::alloc {
    using ::alloc::AlignValue;
    using ::alloc::NewHandler;
    using ::alloc::NoThrowTag;
    using ::alloc::DestroyingDeleteTag;
    using ::alloc::NoThrow;
    using ::alloc::DestroyingDelete;

    using ::alloc::HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE;
    using ::alloc::HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE;

    using ::alloc::BadAllocationException;
    using ::alloc::BadArrayNewLengthException;
}
