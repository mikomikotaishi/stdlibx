#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Kernel control sockets, the AF_SYSTEM/SYSPROTO_CONTROL family - <sys/kern_control.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/kern_control.h>)
    using ControlInfo = ::ctl_info;
    using SocketAddressControl = ::sockaddr_ctl;
    #endif
}
