#pragma once

using stdx::text::CharTraits;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicInputStream = std::basic_istream<Char, Traits>;

    using InputStream = std::istream;
    using WideInputStream = std::wistream;

    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicInputOutputStream = std::basic_iostream<Char, Traits>;

    using InputOutputStream = std::iostream;
    using WideInputOutputStream = std::wiostream;
}
