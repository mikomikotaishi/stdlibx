#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::malloc;
    using alloc::mem::aligned_alloc;
    using alloc::mem::calloc;
    using alloc::mem::realloc;
    using alloc::mem::free;
}
