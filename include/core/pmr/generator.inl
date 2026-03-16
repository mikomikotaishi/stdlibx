#pragma once

/**
 * @namespace core::pmr::ranges
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace core::pmr::ranges {
    #ifdef __cpp_lib_generator
    template <typename Ref, typename V = void>
    using Generator = std::pmr::generator<Ref, V>;
    #endif
}
