#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if defined(__unix__) && __has_include(<sys/resource.h>)
    using ::getrlimit;
    using ::getrlimit64;
    using ::setrlimit;
    using ::setrlimit64;
    using ::getrusage;
    using ::getpriority;
    using ::setpriority;
    #endif
}
