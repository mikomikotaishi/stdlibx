#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::getrlimit;
    using ::getrlimit64;
    using ::setrlimit;
    using ::setrlimit64;
    using ::getrusage;
    using ::getpriority;
    using ::setpriority;
    #endif
}
