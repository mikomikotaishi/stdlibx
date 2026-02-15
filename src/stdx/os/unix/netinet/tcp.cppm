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

export module stdx:os.unix.netinet.tcp;

/**
 * @namespace stdx::os::unix::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::netinet {
    #ifdef __unix__
    using TcpHeader = ::tcphdr;

    enum class TcpCongestionAlgorithmState: unsigned char {
        OPEN = 0,
        DISORDER = 1,
        CWR = 2,
        RECOVERY = 3,
        LOSS = 4
    };

    using TcpInfo = ::tcp_info;

    using TcpMd5Signature = ::tcp_md5sig;
    using TcpRepairOption = ::tcp_repair_opt;

    using TcpCookieTransactions = ::tcp_cookie_transactions;
    using TcpRepairWindow = ::tcp_repair_window;
    using TcpZeroCopyRecieve = ::tcp_zerocopy_receive;
    #endif
}
