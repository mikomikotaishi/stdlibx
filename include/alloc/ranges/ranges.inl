#pragma once

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

    template <Movable V, typename CharT, typename Traits = CharTraits<CharT>>
    using BasicInputStreamView = std::ranges::basic_istream_view<V, CharT, Traits>;

    namespace types {
        template <typename T>
        using InputStream = decltype(std::ranges::views::istream<T>);
    }

    namespace views {
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
