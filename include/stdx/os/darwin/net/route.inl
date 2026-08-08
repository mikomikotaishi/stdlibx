#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief The PF_ROUTE routing socket - <net/route.h>.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/route.h>)
    using RouteMessageHeader = ::rt_msghdr;
    using RouteMessageHeader2 = ::rt_msghdr2;
    using RouteMetrics = ::rt_metrics;
    using InterfaceMessageHeader = ::if_msghdr;
    using InterfaceAddressMessageHeader = ::ifa_msghdr;
    using InterfaceMulticastMessageHeader = ::ifma_msghdr;
    #endif
}
