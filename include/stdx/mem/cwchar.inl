#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using ::alloc::mem::wmemchr;
    using ::alloc::mem::wmemcmp;
    using ::alloc::mem::wmemset;
    using ::alloc::mem::wmemcpy;
    using ::alloc::mem::wmemmove;
}
