#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using ::alloc::mem::memchr;
    using ::alloc::mem::memcmp;
    using ::alloc::mem::memset;
    using ::alloc::mem::memcpy;
    using ::alloc::mem::memmove;
}
