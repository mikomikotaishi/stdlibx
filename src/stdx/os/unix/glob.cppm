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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.glob;
#else
export module stdlibx:os.unix.glob;
#endif

export import :os.unix.glob.constants;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using Stat = ::stat;
    using Stat64 = ::stat64;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using Glob_t = ::glob_t;
    #endif

    using Glob = ::glob_t;

    using ::glob;
    using ::globfree;
    using ::glob64;
    using ::globfree64;

    using ::glob_pattern_p;
    #endif
}
