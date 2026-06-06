#pragma once

/**
 * @namespace stdx::os::unix::arpa
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::arpa {
    #if defined(__unix__) && __has_include(<arpa/inet.h>)
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
