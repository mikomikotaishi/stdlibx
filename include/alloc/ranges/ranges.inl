#pragma once

using core::ranges::Range;

using alloc::text::CharTraits;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace alloc::ranges {
    template <Range R, typename Alloc = Allocator<ByteUnit>>
    using ElementsOf = std::ranges::elements_of<R, Alloc>;

    template <Movable V, typename Char, typename Traits = CharTraits<Char>>
    using BasicInputStreamView = std::ranges::basic_istream_view<V, Char, Traits>;

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
