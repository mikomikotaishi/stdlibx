#pragma once

using core::Movable;
using core::ranges::Range;

using alloc::Allocator;
using alloc::CharTraits;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace alloc::ranges {
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
}

export namespace alloc {
    namespace views = alloc::ranges;
}
