#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char>
    using BasicInputStream = std::basic_istream<Char>;

    using InputStream = std::istream;
    using WideInputStream = std::wistream;

    template <typename Char>
    using BasicInputOutputStream = std::basic_iostream<Char>;
    
    using InputOutputStream = std::iostream;
    using WideInputOutputStream = std::wiostream;
}
