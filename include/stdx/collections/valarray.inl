#pragma once

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename T>
    using ValArray = std::valarray<T>;
    
    template <typename T>
    using SliceArray = std::slice_array<T>;

    template <typename T>
    using GeneralSliceArray = std::gslice_array<T>;

    template <typename T>
    using MaskArray = std::mask_array<T>;

    template <typename T>
    using IndirectArray = std::indirect_array<T>;

    using Slice = std::slice;
    using GeneralSlice = std::gslice;
}
