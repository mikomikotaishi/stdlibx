#pragma once

/**
 * @namespace stdx::os::unix::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::netinet {
    #if defined(__unix__) && __has_include(<netinet/tcp.h>)
    using TcpHeader = ::tcphdr;

    using TcpInfo = ::tcp_info;

    using TcpMd5Signature = ::tcp_md5sig;
    using TcpRepairOption = ::tcp_repair_opt;

    using TcpCookieTransactions = ::tcp_cookie_transactions;
    using TcpRepairWindow = ::tcp_repair_window;
    using TcpZeroCopyRecieve = ::tcp_zerocopy_receive;
    #endif
}
