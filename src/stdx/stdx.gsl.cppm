/**
 * @file stdx.gsl.cppm
 * @module stdx:gsl
 * @brief Module file for Guidelines Support Library operations.
 *
 * This file contains all Guidelines Support Library features.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY
#if __has_include(<gsl/gsl>)
#include <gsl/gsl>
#endif

#include <functional>
#include <type_traits>
#include <utility>
#endif

export module stdx:gsl;

#if defined(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY) && __has_include(<gsl/gsl>)
void _expects(bool cond) {
    Expects(cond);
}

void _ensures(bool cond) {
    Ensures(cond);
}

#undef Expects
#undef Ensures
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY
using usize = std::size_t;
#endif

/**
 * @namespace stdx::gsl
 * @brief Wrapper namespace for the Microsoft GSL objects.
 */
export namespace stdx::gsl {
    #if defined(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY) && __has_include(<gsl/gsl>)
    using ::gsl::copy;

    template <typename T>
        requires std::is_pointer_v<T>
    using Owner = ::gsl::owner<T>;

    template <typename T>
    using NonNull = ::gsl::not_null<T>;

    template <typename T>
    using StrictNonNull = ::gsl::strict_not_null<T>;

    template <
        typename T,
        typename U = decltype(std::declval<const T&>().get()),
        bool = std::is_default_constructible_v<std::hash<U>>
    >
    using NonNullHash = ::gsl::not_null_hash<T, U>;

    using ::gsl::swap;
    using ::gsl::make_not_null;
    using ::gsl::make_strict_not_null;
    using ::gsl::operator<<;

    using NarrowingException = ::gsl::narrowing_error;

    using Index = ::gsl::index;

    using ::gsl::narrow;

    inline constexpr usize GSL_DYNAMIC_EXTENT = ::gsl::dynamic_extent;

    template <typename Char, usize Ext = GSL_DYNAMIC_EXTENT>
    using BasicNullTerminatedString = ::gsl::basic_zstring<Char, Ext>;

    using ConstNullTerminatedString = ::gsl::czstring;
    using ConstWideNullTerminatedString = ::gsl::cwzstring;
    using ConstNullTerminatedString16 = ::gsl::cu16zstring;
    using ConstNullTerminatedString32 = ::gsl::cu32zstring;
    using NullTerminatedString = ::gsl::zstring;
    using WideNullTerminatedString = ::gsl::wzstring;
    using NullTerminatedString16 = ::gsl::u16zstring;
    using NullTerminatedString32 = ::gsl::u32zstring;

    template <typename F>
    using FinalAction = ::gsl::final_action<F>;

    void expects(bool cond) {
        _expects(cond);
    }

    void ensures(bool cond) {
        _ensures(cond);
    }

    using ::gsl::finally;
    using ::gsl::narrow_cast;
    using ::gsl::at;
    using ::gsl::swap;
    #endif
}

#if defined(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY) && __has_include(<gsl/gsl>)
export namespace gsl {
    using ::gsl::operator<<;
    using ::gsl::operator==;
    using ::gsl::operator!=;
    using ::gsl::operator<;
    using ::gsl::operator<=;
    using ::gsl::operator>;
    using ::gsl::operator>=;
    using ::gsl::operator-;
    using ::gsl::operator+;
}

#ifndef COMPILER_GCC
export namespace std {
    using std::hash;
}
#endif
#endif
