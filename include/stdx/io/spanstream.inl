#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    #if __has_include(<spanstream>)
    template <typename Char>
    using BasicSpanBuffer = std::basic_spanbuf<Char>;

    template <typename Char>
    using BasicInputSpanStream = std::basic_ispanstream<Char>;

    template <typename Char>
    using BasicOutputSpanStream = std::basic_ospanstream<Char>;

    template <typename Char>
    using BasicSpanStream = std::basic_spanstream<Char>;

    using SpanBuffer = std::spanbuf;
    using WideSpanBuffer = std::wspanbuf;
    using InputSpanStream = std::ispanstream;
    using WideInputSpanstream = std::wispanstream;
    using OutputSpanStream = std::ospanstream;
    using WideOutputSpanStream = std::wospanstream;

    using SpanStream = std::spanstream;

    using WideSpanStream = std::wspanstream;

    using std::swap;
    #endif
}
