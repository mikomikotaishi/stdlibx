#pragma once

#undef EXIT_SUCCESS
#undef EXIT_FAILURE
#undef MB_CUR_MAX
#undef NULL
#undef RAND_MAX

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::i8;
    using ::core::prelude::i16;
    using ::core::prelude::i32;
    using ::core::prelude::i64;
    using ::core::prelude::u8;
    using ::core::prelude::u16;
    using ::core::prelude::u32;
    using ::core::prelude::u64;
    #ifdef __STDCPP_FLOAT16_T__
    using ::core::prelude::f16;
    #endif
    using ::core::prelude::f32;
    using ::core::prelude::f64;
    using ::core::prelude::f128;
    #ifdef __STDCPP_BFLOAT16_T__
    using ::core::prelude::bf16;
    #endif
    using ::core::prelude::usize;
    using ::core::prelude::isize;
    using ::core::prelude::intmax;
    using ::core::prelude::uintmax;
    using ::core::prelude::ptrdiff;

    using ::core::prelude::byte;
    using ::core::prelude::char8;
    using ::core::prelude::char16;
    using ::core::prelude::char32;
    using ::core::prelude::wchar;

    using ::core::prelude::c32;
    using ::core::prelude::c64;
    using ::core::prelude::c128;

    using ::core::prelude::ByteUnit;

    using ::core::prelude::NullPointer;
    using ::core::prelude::Void;

    using ::core::prelude::CharacterLike;

    using ::core::prelude::Div;
    using ::core::prelude::LongDiv;
    using ::core::prelude::LongLongDiv;
    using ::core::prelude::IntegerMaxDiv;
    using ::core::prelude::MaximumAlignment;

    using ::core::prelude::EXIT_SUCCESS;
    using ::core::prelude::EXIT_FAILURE;
    using ::core::prelude::MB_CUR_MAX;
    using ::core::prelude::NULL;
}
