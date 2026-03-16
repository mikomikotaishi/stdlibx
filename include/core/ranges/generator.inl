#pragma once

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace core::ranges {
    #ifdef __cpp_lib_generator
    template <typename Ref, typename V = void, typename Allocator = void>
    using Generator = std::generator<Ref, V, Allocator>;
    #endif
}
