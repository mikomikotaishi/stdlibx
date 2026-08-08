#pragma once

/**
 * @namespace stdx::alloc
 * @brief Standard library allocation operations.
 */
export namespace stdx::alloc {
    using std::malloc;
    using std::calloc;
    using std::realloc;
    using std::free;

    #ifndef _WIN32
    using std::aligned_alloc;
    #endif
}
