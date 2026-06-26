#pragma once

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    template <typename CharT>
    using BasicStreamBuffer = std::basic_streambuf<CharT>;

    using StreamBuffer = std::streambuf;
    using WideStreamBuffer = std::wstreambuf;
}
