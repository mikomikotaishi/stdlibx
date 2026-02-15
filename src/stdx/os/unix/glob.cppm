/**
 * @file glob.cppm
 * @module stdx:os.unix.glob
 * @brief Module file for Unix globbing operations.
 *
 * This file contains the implementation of the Unix globbing operations from the POSIX libraries,
 * located in <glob.h>.
 */

module;

#ifdef __unix__
#include <glob.h>
#endif

export module stdx:os.unix.glob;

export import :os.unix.glob.constants;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using Stat = ::stat;
    using Stat64 = ::stat64;

    using Glob = ::glob_t;

    using ::glob;
    using ::globfree;
    using ::glob64;
    using ::globfree64;

    using ::glob_pattern_p;
    #endif
}
