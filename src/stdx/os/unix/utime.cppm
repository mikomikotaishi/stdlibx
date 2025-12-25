/**
 * @file utime.cppm
 * @module stdx:os.unix.utime
 * @brief Module file for Unix inode access operations.
 *
 * This file contains the implementation of the Unix inode access operations from the POSIX libraries,
 * located in <utime.h>.
 */

module;

#ifdef __unix__
#include <utime.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.utime;
#else
export module stdlibx:os.unix.utime;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using UTimBuf = ::utimbuf;
    #endif

    using UpdateTimeBuffer = ::utimbuf;

    using ::utime;
    #endif
}
