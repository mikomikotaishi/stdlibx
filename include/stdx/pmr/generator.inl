#pragma once

/**
 * @namespace stdx::pmr::ranges
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::ranges {
    #ifdef __cpp_lib_generator
    template <typename Ref, typename V = void>
    using Generator = std::pmr::generator<Ref, V>;
    #endif
}
