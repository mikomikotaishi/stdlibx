/**
 * @file netdb.cppm
 * @module stdx:os.unix.netdb
 * @brief Module file for Unix numeric address translation operations.
 *
 * This file contains the implementation of the Unix numeric address translation operations from the POSIX libraries,
 * located in <netdb.h>.
 */

module;

#ifdef __unix__
#include <netdb.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.netdb;
#else
export module stdlibx:os.unix.netdb;
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
    using ::herror;
    using ::hstrerror;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using HostEnt = ::hostent;
    #endif

    using HostEntry = ::hostent;

    using ::sethostent;
    using ::endhostent;
    using ::gethostent;
    using ::gethostbyaddr;
    using ::gethostbyname;
    using ::gethostbyname2;
    using ::gethostent_r;
    using ::gethostbyaddr_r;
    using ::gethostbyname_r;
    using ::gethostbyname2_r;
    #endif
}
