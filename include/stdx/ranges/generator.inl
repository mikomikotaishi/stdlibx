#pragma once

/**
 * @namespace stdx::ranges
 * @brief Standard library ranges operations.
 */
export namespace stdx::ranges {
    #ifdef __cpp_lib_generator
    template <typename Ref, typename V = void, typename Allocator = void>
    using Generator = std::generator<Ref, V, Allocator>;
    #endif
}
