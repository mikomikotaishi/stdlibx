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

export module stdx:os.unix.netdb;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::herror;
    using ::hstrerror;

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
