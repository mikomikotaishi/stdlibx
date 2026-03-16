#pragma once

/**
 * @namespace stdx::pmr::ranges
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::ranges {
    #ifdef __cpp_lib_generator
    using ::core::pmr::ranges::Generator;
    #endif
}
