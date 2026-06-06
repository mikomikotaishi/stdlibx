#pragma once

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_inplace_vector
    template <typename T, usize N>
    using StaticVector = std::inplace_vector<T, N>;
    #endif
}
