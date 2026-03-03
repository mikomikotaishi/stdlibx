#pragma once

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    #ifdef __cpp_lib_generator
    using ::core::ranges::Generator;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using ::core::ranges::pmr::Generator;
    }
    #endif
}
