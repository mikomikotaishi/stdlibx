#pragma once

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    #ifdef __cpp_lib_ranges_generate_random
    using std::ranges::generate_random;
    #endif
}
