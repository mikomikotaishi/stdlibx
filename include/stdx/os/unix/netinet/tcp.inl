#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::netinet::captured
 * @brief The numeric values of the <netinet/tcp.h> macros, read where they exist.
 *
 * TCP_NODELAY is 1 everywhere. The keepalive knobs are where the platforms part:
 * Linux spells the idle time TCP_KEEPIDLE, Darwin spells it TCP_KEEPALIVE, and
 * neither defines the other's name - so that one is not a differing value but a
 * differing interface, and Darwin's lives in stdx::os::darwin::netinet.
 */
namespace stdx::os::unix::netinet::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netinet/tcp.h>)
    constexpr auto TCP_NODELAY_VALUE = TCP_NODELAY;
    constexpr auto TCP_MAXSEG_VALUE = TCP_MAXSEG;
    constexpr auto TCP_KEEPINTVL_VALUE = TCP_KEEPINTVL;
    constexpr auto TCP_KEEPCNT_VALUE = TCP_KEEPCNT;
    constexpr auto TCP_FASTOPEN_VALUE = TCP_FASTOPEN;

    /// Linux-only knobs. TCP_CORK's nearest Darwin equivalent is TCP_NOPUSH.
    #ifdef __linux__
    constexpr auto TCP_CORK_VALUE = TCP_CORK;
    constexpr auto TCP_KEEPIDLE_VALUE = TCP_KEEPIDLE;
    constexpr auto TCP_QUICKACK_VALUE = TCP_QUICKACK;
    #endif
    #endif
}

/**
 * @namespace stdx::os::unix::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::netinet {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netinet/tcp.h>)
    using TcpHeader = ::tcphdr;

    /**
     * @brief Linux's TCP introspection and repair structures.
     *
     * Only tcphdr is portable. Everything below is Linux kernel interface:
     * tcp_info has no Darwin counterpart with the same shape (Darwin offers
     * tcp_connection_info, which stdx::os::darwin would carry), and the repair,
     * MD5-signature and zero-copy structures have no equivalent at all.
     */
    #ifdef __linux__
    using TcpInfo = ::tcp_info;

    using TcpMd5Signature = ::tcp_md5sig;
    using TcpRepairOption = ::tcp_repair_opt;

    using TcpCookieTransactions = ::tcp_cookie_transactions;
    using TcpRepairWindow = ::tcp_repair_window;
    using TcpZeroCopyRecieve = ::tcp_zerocopy_receive;
    #endif
    #endif
}
