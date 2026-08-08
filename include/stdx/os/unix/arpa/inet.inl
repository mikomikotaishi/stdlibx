#pragma once

/**
 * @namespace stdx::os::unix::arpa
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::arpa {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<arpa/inet.h>)
    #ifdef __APPLE__
    // Darwin provides the byte-order helpers only as function-like macros,
    // so wrap them as real functions. The parenthesized names keep the
    // declarations from expanding the macros; the bodies expand them on
    // purpose. Not inline: the expansion references TU-local helpers, which
    // an exported inline function may not touch.
    __uint16_t (htons)(__uint16_t value) noexcept { return htons(value); }
    __uint32_t (htonl)(__uint32_t value) noexcept { return htonl(value); }
    __uint16_t (ntohs)(__uint16_t value) noexcept { return ntohs(value); }
    __uint32_t (ntohl)(__uint32_t value) noexcept { return ntohl(value); }
    #else
    using ::htons;
    using ::htonl;
    using ::ntohs;
    using ::ntohl;
    #endif

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
