#pragma once

using stdx::text::CharTraits;

/**
 * @namespace stdx::io
 * @brief Standard library IO operations.
 */
export namespace stdx::io {
    #if __has_include(<spanstream>)
    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicSpanBuffer = std::basic_spanbuf<Char, Traits>;

    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicInputSpanStream = std::basic_ispanstream<Char, Traits>;

    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicOutputSpanStream = std::basic_ospanstream<Char, Traits>;

    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicSpanStream = std::basic_spanstream<Char, Traits>;

    using SpanBuffer = std::spanbuf;
    using WideSpanBuffer = std::wspanbuf;
    using InputSpanStream = std::ispanstream;
    using WideInputSpanstream = std::wispanstream;
    using OutputSpanStream = std::ospanstream;
    using WideOutputSpanStream = std::wospanstream;

    using SpanStream = std::spanstream;

    using WideSpanStream = std::wspanstream;
    #endif
}
