#pragma once

/**
 * @namespace stdx::core::gsl
 * @brief Wrapper namespace for the Microsoft GSL objects.
 */
export namespace stdx::core::gsl {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY
    using ::core::gsl::Owner;
    using ::core::gsl::NonNull;
    using ::core::gsl::StrictNonNull;
    using ::core::gsl::NonNullHash;
    using ::core::gsl::NarrowingException;
    using ::core::gsl::Index;
    using ::core::gsl::BasicNullTerminatedString;
    using ::core::gsl::ConstWideNullTerminatedString;
    using ::core::gsl::ConstNullTerminatedString16;
    using ::core::gsl::ConstNullTerminatedString32;
    using ::core::gsl::NullTerminatedString;
    using ::core::gsl::WideNullTerminatedString;
    using ::core::gsl::NullTerminatedString16;
    using ::core::gsl::NullTerminatedString32;
    using ::core::gsl::FinalAction;

    using ::core::gsl::GSL_DYNAMIC_EXTENT;

    using ::core::gsl::expects;
    using ::core::gsl::ensures;

    using ::core::gsl::copy;
    using ::core::gsl::swap;
    using ::core::gsl::make_not_null;
    using ::core::gsl::make_strict_not_null;
    using ::core::gsl::narrow;
    using ::core::gsl::finally;
    using ::core::gsl::narrow_cast;
    using ::core::gsl::at;
    using ::core::gsl::operator<<;
    #endif
}
