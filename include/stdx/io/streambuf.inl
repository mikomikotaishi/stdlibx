#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename CharT>
    using BasicStreamBuffer = std::basic_streambuf<CharT>;

    using StreamBuffer = std::streambuf;
    using WideStreamBuffer = std::wstreambuf;

    using std::swap;
}
