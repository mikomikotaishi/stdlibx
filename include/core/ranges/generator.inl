#pragma once

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace core::ranges {
    #if __has_include(<generator>)
    template <typename Ref, typename V = void, typename Allocator = void>
    using Generator = std::generator<Ref, V, Allocator>;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename Ref, typename V = void>
        using Generator = std::pmr::generator<Ref, V>;
    }
    #endif
}
