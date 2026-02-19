#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename CharT>
    using BasicInputStream = std::basic_istream<CharT>;

    using InputStream = std::istream;
    using WideInputStream = std::wistream;

    template <typename CharT>
    using BasicInputOutputStream = std::basic_iostream<CharT>;
    
    using InputOutputStream = std::iostream;
    using WideInputOutputStream = std::wiostream;
}
