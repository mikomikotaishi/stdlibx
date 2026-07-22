#pragma once

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char>
    using BasicStreamBuffer = std::basic_streambuf<Char>;

    using StreamBuffer = std::streambuf;
    using WideStreamBuffer = std::wstreambuf;
}
