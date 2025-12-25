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

import core;

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

    using ::core::char8;
    using ::core::char16;
    using ::core::char32;

    using ::core::c32;
    using ::core::c64;
    using ::core::c128;

    using ::core::byte;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::NullPtr;
    using ::core::NullPtr_t;
    using ::core::NullPointer_t;
    #endif

    using ::core::NullPointer;
    using ::core::Void;
    using ::core::Boolean;
    using ::core::Short;
    using ::core::Integer;
    using ::core::Long;
    using ::core::LongLong;
    using ::core::UnsignedShort;
    using ::core::UnsignedInteger;
    using ::core::UnsignedLong;
    using ::core::UnsignedLongLong;
    using ::core::Integer8;
    using ::core::Integer16;
    using ::core::Integer32;
    using ::core::Integer64;
    using ::core::UnsignedInteger8;
    using ::core::UnsignedInteger16;
    using ::core::UnsignedInteger32;
    using ::core::UnsignedInteger64;
    using ::core::Byte;
    using ::core::SignedSize;
    using ::core::UnsignedSize;
    using ::core::Character;
    using ::core::UnsignedCharacter;
    using ::core::Character8;
    using ::core::Character16;
    using ::core::Character32;
    using ::core::WideCharacter;
    using ::core::Float;
    using ::core::Double;
    using ::core::LongDouble;
    // using ::core::Float16;
    using ::core::Float32;
    using ::core::Float64;
    using ::core::Float128;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::Div_t;
    using ::core::LDiv_t;
    using ::core::LLDiv_t;
    using ::core::LDiv;
    using ::core::LLDiv;
    using ::core::LongDiv_t;
    using ::core::LongLongDiv_t;
    using ::core::IntegerMaxDiv_t;
    using ::core::MaxAlign;
    using ::core::MaxAlign_t;
    using ::core::MaximumAlignment_t;
    #endif
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
