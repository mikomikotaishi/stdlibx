#pragma once

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    #ifdef __cpp_lib_generator
    using ::core::ranges::Generator;
    #endif
}
