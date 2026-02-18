#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::AlignValue;
    using alloc::mem::NewHandler;
    using alloc::mem::NoThrow;
    using alloc::mem::DestroyingDelete;

    using alloc::mem::HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE;
    using alloc::mem::HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE;

    using alloc::mem::BadAllocationException;
    using alloc::mem::BadArrayNewLengthException;

    using alloc::mem::get_new_handler;
    using alloc::mem::set_new_handler;
    
    using alloc::mem::launder;
}
