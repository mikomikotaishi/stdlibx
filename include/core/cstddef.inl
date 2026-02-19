#pragma once

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
    using usize = std::size_t;
    using isize = std::intptr_t;
    using intmax = std::intmax_t;
    using uintmax = std::uintmax_t;
    using ptrdiff = std::ptrdiff_t;

    #ifdef __STDCPP_FLOAT16_T__
    using f16 = std::float16_t;
    #endif
    #ifdef __STDCPP_BFLOAT16_T__
    using f32 = std::float32_t;
    #else
    using f32 = float;
    #endif
    #ifdef __STDCPP_FLOAT64_T__
    using f64 = std::float64_t;
    #else
    using f64 = double;
    #endif
    #ifdef __STDCPP_FLOAT128_T__
    using f128 = std::float128_t;
    #else
    using f128 = long double;
    #endif
    #ifdef __STDCPP_BFLOAT16_T__
    using bf16 = std::bfloat16_t;
    #endif

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

    using Div = std::div_t;
    using LongDiv = std::ldiv_t;
    using LongLongDiv = std::lldiv_t;
    using IntegerMaxDiv = std::imaxdiv_t;
    using MaximumAlignment = std::max_align_t;
}
