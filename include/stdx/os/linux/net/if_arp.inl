#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/if_arp.h>)
    using ArpHeader = ::arphdr;
    using ArpRequest = ::arpreq;
    using ArpRequestOld = ::arpreq_old;
    using ArpDaemonRequest = ::arpd_request;
    #endif
}
