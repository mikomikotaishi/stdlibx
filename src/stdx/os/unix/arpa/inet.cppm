/**
 * @file inet.cppm
 * @module stdx:os.unix.arpa.inet
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in <arpa/inet.h>.
 */

module;

#ifdef __unix__
#include <arpa/inet.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.arpa.inet;
#else
export module stdlibx:os.unix.arpa.inet;
#endif

/**
 * @namespace stdx::os::unix::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::arpa {
#else 
export namespace stdlibx::os::unix::arpa {
#endif
    #ifdef __unix__
    using ::inet_addr;
    using ::inet_lnaof;
    using ::inet_makeaddr;
    using ::inet_netof;
    using ::inet_network;
    using ::inet_ntoa;
    using ::inet_pton;
    using ::inet_ntop;
    using ::inet_aton;
    using ::inet_neta;
    using ::inet_net_ntop;
    using ::inet_net_pton;
    using ::inet_nsap_addr;
    using ::inet_nsap_ntoa;
    #endif
}
