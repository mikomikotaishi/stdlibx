/**
 * @file pwd.cppm
 * @module stdx:os.unix.pwd
 * @brief Module file for Unix password access/control operations.
 *
 * This file contains the implementation of the Unix password access/control operations from the POSIX libraries,
 * located in <pwd.h>.
 */

module;

#ifdef __unix__
#include <pwd.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.pwd;
#else
export module stdlibx:os.unix.pwd;
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
    using Passwd = ::passwd;
    #endif

    using Password = ::passwd;

    using ::setpwent;
    using ::endpwent;
    using ::getpwent;
    using ::fgetpwent;
    using ::putpwent;
    using ::getpwuid;
    using ::getpwnam;
    using ::getpwent_r;
    using ::getpwuid_r;
    using ::getpwnam_r;
    using ::fgetpwent_r;
    using ::getpw;
    #endif
}
