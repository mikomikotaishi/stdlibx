#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<glob.h>)
    using Stat = struct stat;
    using Stat64 = struct stat64;

    using Glob = ::glob_t;

    using ::glob;
    using ::globfree;
    using ::glob64;
    using ::globfree64;

    using ::glob_pattern_p;
    #endif
}
