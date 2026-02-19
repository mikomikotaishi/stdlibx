#pragma once

/**
 * @namespace alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace alloc {
    using std::malloc;
    using std::aligned_alloc;
    using std::calloc;
    using std::realloc;
    using std::free;
}
