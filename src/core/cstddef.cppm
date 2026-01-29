/**
 * @file cstddef.cppm
 * @module core:cstddef
 * @brief Module file for importing various aliases from the standard library.
 * 
 * This file exports various aliases in the standard library.
 */

module;

#include <cinttypes>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

#include "Macros.hpp"

#define STDLIBX_NO_STD_CORE_FEATURES

export module core:cstddef;

import :compare;
import :concepts;
import :limits;
import :optional;
import :string_view;
import :system_error;
import :text.charconv;

/**
 * @internal
 * @namespace _detail::core
 * @brief Wrapper namespace for standard library core operations.
 * Detail namespace - not to be exported for external use
 */
namespace _detail::core {
    constexpr decltype(EXIT_SUCCESS) ExitSuccess = EXIT_SUCCESS;
    constexpr decltype(EXIT_FAILURE) ExitFailure = EXIT_FAILURE;
    const decltype(MB_CUR_MAX) MultibyteCurrentMax = MB_CUR_MAX;
    constexpr decltype(NULL) Null = NULL;
    constexpr decltype(RAND_MAX) RandMax = RAND_MAX;
}

#undef EXIT_SUCCESS
#undef EXIT_FAILURE
#undef MB_CUR_MAX
#undef NULL
#undef RAND_MAX

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    // using f16 = std::float16_t;
    // using f32 = std::float32_t;
    // using f64 = std::float64_t;
    // using f128 = std::float128_t;
    // using bf16 = std::bfloat16_t;
    using f32 = float;
    using f64 = double;
    using f128 = long double;
    using usize = std::size_t;
    using isize = std::intptr_t;

    using byte = unsigned char;
    using char8 = char8_t;
    using char16 = char16_t;
    using char32 = char32_t;
    using wchar = wchar_t;

    using c32 = std::complex<float>;
    using c64 = std::complex<double>;
    using c128 = std::complex<long double>;

    using ByteUnit = std::byte;

    using NullPointer = std::nullptr_t;


    using Void = void;

    #include "Numbers.inl"

    using Div = std::div_t;
    using LongDiv = std::ldiv_t;
    using LongLongDiv = std::lldiv_t;
    using IntegerMaxDiv = std::imaxdiv_t;
    using MaximumAlignment = std::max_align_t;

    constexpr decltype(_detail::core::ExitSuccess) EXIT_SUCCESS = _detail::core::ExitSuccess;
    constexpr decltype(_detail::core::ExitFailure) EXIT_FAILURE = _detail::core::ExitFailure;
    const decltype(_detail::core::MultibyteCurrentMax) MB_CUR_MAX = _detail::core::MultibyteCurrentMax;
    constexpr decltype(_detail::core::Null) NULL = _detail::core::Null;
    constexpr decltype(_detail::core::RandMax) RAND_MAX = _detail::core::RandMax;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
