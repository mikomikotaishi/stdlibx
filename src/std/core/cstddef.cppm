/**
 * @file cstddef.cppm
 * @module std:core.cstddef
 * @brief Module file for importing various aliases from the standard library.
 * 
 * This file exports various aliases in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.cstddef;
#else
export module stdlib:core.cstddef;
#endif

import :core.exception;
import :fmt.format;

import core;
import alloc;

using alloc::String;

using core::Errc;
using core::FloatingPoint;
using core::Integral;
using core::NumericLimits;
using core::Optional;
using core::StringView;
using core::StrongOrdering;
using core::nullopt;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
namespace fmt = std::fmt;
#else
namespace fmt = stdlib::fmt;
#endif

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    using ::core::i8;
    using ::core::i16;
    using ::core::i32;
    using ::core::i64;
    using ::core::u8;
    using ::core::u16;
    using ::core::u32;
    using ::core::u64;
    // using ::core::f16;
    // using ::core::f32;
    // using ::core::f64;
    // using ::core::f128;
    // using ::core::bf16;
    using ::core::f32;
    using ::core::f64;
    using ::core::f128;
    using ::core::usize;
    using ::core::isize;

    using ::core::byte;
    using ::core::char8;
    using ::core::char16;
    using ::core::char32;
    using ::core::wchar;

    using ::core::c32;
    using ::core::c64;
    using ::core::c128;

    using ::core::ByteUnit;


    using ::core::NullPointer;
    using ::core::Void;

    #include "Numbers.inl"

    using ::core::Div;
    using ::core::LongDiv;
    using ::core::LongLongDiv;
    using ::core::IntegerMaxDiv;
    using ::core::MaximumAlignment;

    using ::core::EXIT_SUCCESS;
    using ::core::EXIT_FAILURE;
    using ::core::MB_CUR_MAX;
    using ::core::NULL;
    using ::core::RAND_MAX;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
