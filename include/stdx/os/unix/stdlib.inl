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

    // Darwin: setenv/unsetenv/putenv are POSIX and present; clearenv is not.
    // No Darwin header declares environ (see environ(7)), so declare it here;
    // the extern "C" linkage binds it to the real libc symbol.
    #if defined(__APPLE__) && !defined(__unix__) && __has_include(<stdlib.h>)
    using ::setenv;
    using ::unsetenv;
    using ::putenv;

    extern "C" char** environ;
    #endif
}
