/**
 * @file nameser.cppm
 * @module stdx:os.linux.arpa.nameser
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in <arpa/nameser.h>.
 */

module;

#ifdef __linux__
#include <arpa/nameser.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa.nameser;
#else
export module stdlibx:os.linux.arpa.nameser;
#endif

export import :os.linux.arpa.nameser.constants;

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::arpa {
#else 
export namespace stdlibx::os::linux::arpa {
#endif
    #ifdef __linux__
    using NsTsigKey = ::ns_tsig_key;
    using NsTcpTsigState = ::ns_tcp_tsig_state;

    using ::ns_msg_getflag;
    using ::ns_get16;
    using ::ns_get32;
    using ::ns_put16;
    using ::ns_put32;
    using ::ns_initparse;
    using ::ns_skiprr;
    using ::ns_parserr;
    using ::ns_sprintrr;
    using ::ns_sprintrrf;
    using ::ns_format_ttl;
    using ::ns_parse_ttl;
    using ::ns_datetosecs;
    using ::ns_name_ntol;
    using ::ns_name_ntop;
    using ::ns_name_pton;
    using ::ns_name_unpack;
    using ::ns_name_uncompress;
    using ::ns_name_compress;
    using ::ns_name_skip;
    using ::ns_name_rollback;
    using ::ns_samedomain;
    using ::ns_subdomain;
    using ::ns_makecanon;
    using ::ns_samename;
    #endif
}
