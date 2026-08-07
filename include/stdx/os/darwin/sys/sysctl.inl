#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief The sysctl management interface - <sys/sysctl.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/sysctl.h>)
    using ClockInfo = ::clockinfo;
    using LoadAverage = ::loadavg;
    using KernelInfoProc = ::kinfo_proc;

    using ::sysctl;
    using ::sysctlbyname;
    using ::sysctlnametomib;
    #endif
}
