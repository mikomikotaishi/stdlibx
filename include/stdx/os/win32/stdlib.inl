#pragma once

/**
 * @namespace stdx::os::win32
 * @brief C runtime process-environment access (<stdlib.h>).
 *
 * The MSVC CRT keeps its own environment block (what getenv reads); these are
 * the entry points that stay in sync with it, unlike the Win32 Environment
 * Strings APIs. Mirrors the unix stdlib.inl (setenv/unsetenv).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32)
    // Environment mutation (keep the CRT environment in sync)
    using ::_putenv_s;
    using ::_wputenv_s;

    // The CRT environment block
    using ::_environ;
    using ::_wenviron;
    #endif
}
