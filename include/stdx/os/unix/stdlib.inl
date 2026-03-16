#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX stdlib extensions.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::setenv;
    using ::unsetenv;
    using ::clearenv;
    using ::putenv;
    #endif
}
