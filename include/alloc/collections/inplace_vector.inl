#pragma once

/**
 * @namespace core::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace core::collections {
    #if __has_include(<inplace_vector>)
    template <typename T, usize N>
    using InPlaceVector = std::inplace_vector<T, N>;

    using std::erase;
    using std::erase_if;
    #endif
}
