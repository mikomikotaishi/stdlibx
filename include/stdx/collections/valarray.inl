#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
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

    using std::operator+;
    using std::operator-;
    using std::operator*;
    using std::operator/;
    using std::operator%;
    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator<<;
    using std::operator>>;
    using std::operator&&;
    using std::operator||;

    using std::operator==;
    using std::operator!=;
    using std::operator<;
    using std::operator<=;
    using std::operator>;
    using std::operator>=;
    using std::operator<=>;

    using std::abs;
    using std::exp;
    using std::log;
    using std::log10;
    using std::pow;
    using std::sqrt;
    using std::sin;
    using std::cos;
    using std::tan;
    using std::asin;
    using std::acos;
    using std::atan;
    using std::atan2;
    using std::sinh;
    using std::cosh;
    using std::tanh;

    using std::hash;
    using std::swap;
}
