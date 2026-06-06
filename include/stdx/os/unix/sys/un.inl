#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if defined(__unix__) && __has_include(<sys/un.h>)
    using UnixSocketAddress = ::sockaddr_un;
    #endif
}
