/**
 * @file utsname.cppm
 * @module stdx:os.unix.sys.utsname
 * @brief Module file for Unix operating system information operations.
 *
 * This file contains the implementation of the Unix operating system information operations from the POSIX libraries,
 * located in <sys/utsname.h>.
 */

module;

#ifdef __unix__
#include <sys/utsname.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.utsname;
#else
export module stdlibx:os.unix.sys.utsname;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using UTSName = ::utsname;
    #endif

    using UnixTimeSystemName = ::utsname;

    using ::uname;
    #endif
}
