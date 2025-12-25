/**
 * @file utmpx.cppm
 * @module stdx:os.unix.utmpx
 * @brief Module file for Unix user account database operations.
 *
 * This file contains the implementation of the Unix user account database operations from the POSIX libraries,
 * located in <utmpx.h>.
 */

module;

#ifdef __unix__
#include <utmpx.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.utmpx;
#else
export module stdlibx:os.unix.utmpx;
#endif

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
    using ::setutxent;
    using ::endutxent;
    using ::getutxent;
    using ::getutxid;
    using ::getutxline;
    using ::pututxline;
    using ::utmpxname;
    using ::updwtmpx;
    using ::getutmp;
    using ::getutmpx;
    #endif
}
