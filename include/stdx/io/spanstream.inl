#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    #if __has_include(<spanstream>)
    template <typename CharT>
    using BasicSpanBuffer = std::basic_spanbuf<CharT>;

    template <typename CharT>
    using BasicInputSpanStream = std::basic_ispanstream<CharT>;

    template <typename CharT>
    using BasicOutputSpanStream = std::basic_ospanstream<CharT>;

    template <typename CharT>
    using BasicSpanStream = std::basic_spanstream<CharT>;

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
