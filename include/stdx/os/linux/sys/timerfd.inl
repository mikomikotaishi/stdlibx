#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::timerfd_create;
    using ::timerfd_settime;
    using ::timerfd_gettime;
    #endif
}
