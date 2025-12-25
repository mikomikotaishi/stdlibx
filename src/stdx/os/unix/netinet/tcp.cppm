/**
 * @file tcp.cppm
 * @module stdx:os.unix.netinet.tcp
 * @brief Module file for Unix TCP control operations.
 *
 * This file contains the implementation of the Unix TCP control operations from the POSIX libraries,
 * located in <netinet/tcp.h>.
 */

module;

#ifdef __unix__
#include <netinet/tcp.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.netinet.tcp;
#else
export module stdlibx:os.unix.netinet.tcp;
#endif

/**
 * @namespace stdx::os::unix::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::netinet {
#else 
export namespace stdlibx::os::unix::netinet {
#endif
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using TcpHdr = ::tcphdr;
    #endif
    
    using TcpHeader = ::tcphdr;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    enum class TcpCaState {
        OPEN = 0,
        DISORDER = 1,
        CWR = 2,
        RECOVERY = 3,
        LOSS = 4
    };
    #endif

    enum class TcpCongestionAlgorithmState {
        OPEN = 0,
        DISORDER = 1,
        CWR = 2,
        RECOVERY = 3,
        LOSS = 4
    };

    using TcpInfo = ::tcp_info;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using TcpMd5Sig = ::tcp_md5sig;
    using TcpRepairOpt = ::tcp_repair_opt;
    #endif

    using TcpMd5Signature = ::tcp_md5sig;
    using TcpRepairOption = ::tcp_repair_opt;

    using TcpCookieTransactions = ::tcp_cookie_transactions;
    using TcpRepairWindow = ::tcp_repair_window;
    using TcpZeroCopyRecieve = ::tcp_zerocopy_receive;
    #endif
}
