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
    using ::core::intmax;
    using ::core::uintmax;
    using ::core::ptrdiff;

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

    using ::core::CharacterLike;

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
