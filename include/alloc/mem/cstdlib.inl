#pragma once

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    using std::malloc;
    using std::aligned_alloc;
    using std::calloc;
    using std::realloc;
    using std::free;
}
