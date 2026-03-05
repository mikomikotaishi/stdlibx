#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using ArpHeader = ::arphdr;
    using ArpRequest = ::arpreq;
    using ArpRequestOld = ::arpreq_old;
    using ArpDaemonRequest = ::arpd_request;
    #endif
}
