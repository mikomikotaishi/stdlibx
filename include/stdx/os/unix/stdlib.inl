#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX stdlib extensions.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<stdlib.h>)
    using ::setenv;
    using ::unsetenv;
    using ::clearenv;
    using ::putenv;

    // The process environment block (parallels win32::_environ).
    using ::environ;
    #endif
}
