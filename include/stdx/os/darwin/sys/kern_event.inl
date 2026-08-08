#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Kernel event notifications, the AF_SYSTEM/SYSPROTO_EVENT family - <sys/kern_event.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/kern_event.h>)
    using KernelEventMessage = ::kern_event_msg;
    using KernelEventRequest = ::kev_request;
    using KernelEventVendorCode = ::kev_vendor_code;
    #endif
}
