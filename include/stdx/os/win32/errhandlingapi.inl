#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Error handling and formatting (errhandlingapi.h + winbase formatting).
 *
 * Exception-related types (CONTEXT, EXCEPTION_RECORD, vectored handlers, ...)
 * live in exception.inl; this file is for last-error queries, message
 * formatting, and error-mode controls.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    using ::GetLastError;
    using ::SetLastError;
    using ::RestoreLastError;

    using ::FormatMessageA;
    using ::FormatMessageW;

    using ::GetErrorMode;
    using ::SetErrorMode;
    using ::GetThreadErrorMode;
    using ::SetThreadErrorMode;
    #endif
}
