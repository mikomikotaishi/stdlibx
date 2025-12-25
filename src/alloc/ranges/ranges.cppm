/**
 * @file ranges.cppm
 * @module alloc:ranges.ranges
 * @brief Module file for standard library ranges operations.
 *
 * This file contains the implementation of the ranges operations in the standard library.
 */

module;

#if __has_include(<ranges>)
#include <ranges>
#endif

export module alloc:ranges.ranges;

import core;

import :string;
import :mem.memory;

using core::Movable;
#if __has_include(<ranges>)
using core::ranges::Range;
#endif

using alloc::CharTraits;
using alloc::mem::Allocator;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace alloc::ranges {
    #if __has_include(<ranges>)
    template <Range R, typename Alloc = Allocator<std::byte>>
    using ElementsOf = std::ranges::elements_of<R, Alloc>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <Movable V, typename CharT, typename Traits = CharTraits<CharT>>
    using BasicIStreamView = std::ranges::basic_istream_view<V, CharT, Traits>;
    #endif

    template <Movable V, typename CharT, typename Traits = CharTraits<CharT>>
    using BasicInputStreamView = std::ranges::basic_istream_view<V, CharT, Traits>;

    namespace types {
        template <typename T>
        using InputStream = decltype(std::ranges::views::istream<T>);
    }

    namespace views {
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        template <typename T>
        inline constexpr types::InputStream<T> IStream = std::ranges::views::istream<T>;
        #endif
        template <typename T>
        inline constexpr types::InputStream<T> InputStream = std::ranges::views::istream<T>;
    }
    #endif
}

export namespace alloc {
    #if __has_include(<ranges>)
    namespace views = alloc::ranges;
    #endif
}
