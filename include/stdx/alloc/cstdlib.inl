#pragma once

/**
 * @namespace stdx::alloc
 * @brief Standard library allocation operations.
 */
export namespace stdx::alloc {
    using std::malloc;
    using std::aligned_alloc;
    using std::calloc;
    using std::realloc;
    using std::free;
}
