/**
 * @file epoll.cppm
 * @module stdx:os.linux.sys.epoll
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/epoll.h>.
 */

module;

#ifdef __linux__
#include <sys/epoll.h>
#endif

export module stdx:os.linux.sys.epoll;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    enum class EpollEvents : uint32_t {
        IN = 0x001,
        PRI = 0x002,
        OUT = 0x004,
        RD_NORM = 0x040,
        RD_BAND = 0x080,
        WR_NORM = 0x100,
        WR_BAND = 0x200,
        MSG = 0x400,
        ERR = 0x008,
        HUP = 0x010,
        RD_HUP = 0x2000,
        EXCLUSIVE = 1u << 28,
        WAKEUP = 1u << 29,
        ONE_SHOT = 1u << 30,
        EDGE_TRIGGERED = 1u << 31
    };

    using EpollData = ::epoll_data;
    using EpollEvent = ::epoll_event;
    using EpollParams = ::epoll_params;

    using ::epoll_create;
    using ::epoll_create1;
    using ::epoll_ctl;
    using ::epoll_wait;
    using ::epoll_pwait;
    using ::epoll_pwait2;
    #endif
}
