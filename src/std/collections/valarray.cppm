/**
 * @file valarray.cppm
 * @module std:collections.valarray
 * @brief Module file for standard library valarray operations.
 *
 * This file contains the implementation of the valarray operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.valarray;
#else
export module stdlib:collections.valarray;
#endif

import alloc;

/**
 * @namespace std::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::collections {
#else 
export namespace stdlib::collections {
#endif
    using alloc::collections::ValArray;
    using alloc::collections::SliceArray;
    using alloc::collections::GSliceArray;
    using alloc::collections::MaskArray;
    using alloc::collections::IndirectArray;
    using alloc::collections::Slice;
    using alloc::collections::GSlice;

    using alloc::collections::operator+;
    using alloc::collections::operator-;
    using alloc::collections::operator*;
    using alloc::collections::operator/;
    using alloc::collections::operator%;
    using alloc::collections::operator&;
    using alloc::collections::operator|;
    using alloc::collections::operator^;
    using alloc::collections::operator<<;
    using alloc::collections::operator>>;
    using alloc::collections::operator&&;
    using alloc::collections::operator||;

    using alloc::collections::operator==;
    using alloc::collections::operator!=;
    using alloc::collections::operator<;
    using alloc::collections::operator<=;
    using alloc::collections::operator>;
    using alloc::collections::operator>=;
    using alloc::collections::operator<=>;

    using alloc::collections::abs;
    using alloc::collections::exp;
    using alloc::collections::log;
    using alloc::collections::log10;
    using alloc::collections::pow;
    using alloc::collections::sqrt;
    using alloc::collections::sin;
    using alloc::collections::cos;
    using alloc::collections::tan;
    using alloc::collections::asin;
    using alloc::collections::acos;
    using alloc::collections::atan;
    using alloc::collections::atan2;
    using alloc::collections::sinh;
    using alloc::collections::cosh;
    using alloc::collections::tanh;

    using alloc::collections::hash;
    using alloc::collections::swap;
}
