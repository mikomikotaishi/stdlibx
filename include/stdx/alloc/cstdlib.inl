#pragma once

/**
 * @namespace stdx::alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace stdx::alloc {
    using ::alloc::malloc;
    using ::alloc::aligned_alloc;
    using ::alloc::calloc;
    using ::alloc::realloc;
    using ::alloc::free;
}