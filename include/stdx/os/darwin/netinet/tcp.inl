#pragma once

/**
 * @internal
 * @namespace stdx::os::darwin::captured
 * @brief Darwin's <netinet/tcp.h> spellings, read where they exist.
 *
 * TCP_NODELAY is POSIX-common and lives in unix.constants.inl. The keepalive
 * knobs are not: Darwin defines TCP_KEEPALIVE for the idle time where Linux
 * defines TCP_KEEPIDLE, and neither platform defines the other's name, so this
 * is a genuine difference in the interface rather than in a number.
 */
namespace stdx::os::darwin::captured {
    #if defined(__APPLE__) && __has_include(<netinet/tcp.h>)
    constexpr auto TCP_KEEPALIVE_VALUE = TCP_KEEPALIVE;
    constexpr auto TCP_KEEPINTVL_VALUE = TCP_KEEPINTVL;
    constexpr auto TCP_KEEPCNT_VALUE = TCP_KEEPCNT;
    #endif
}
