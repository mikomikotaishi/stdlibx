#pragma once

/**
 * @namespace core::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace core::collections {
    #ifdef __cpp_lib_inplace_vector
    template <typename T, usize N>
    using StaticVector = std::inplace_vector<T, N>;

    using std::erase;
    using std::erase_if;
    #endif
}
