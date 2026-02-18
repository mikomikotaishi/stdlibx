#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    inline OutputStream& Cout = std::cout;
    inline InputStream& Cin = std::cin;
    inline OutputStream& Cerr = std::cerr;
    inline OutputStream& Clog = std::clog;

    inline WideOutputStream& WideCout = std::wcout;
    inline WideInputStream& WideCin = std::wcin;
    inline WideOutputStream& WideCerr = std::wcerr;
    inline WideOutputStream& WideClog = std::wclog;
}
