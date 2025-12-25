/**
 * @file inotify.cppm
 * @module stdx:os.linux.sys.inotify
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/inotify.h>.
 */

module;

#ifdef __linux__
#include <sys/inotify.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.inotify;
#else
export module stdlibx:os.linux.sys.inotify;
#endif

export import :os.linux.sys.inotify.constants;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    using INotifyEvent = ::inotify_event;

    using ::inotify_init;
    using ::inotify_init1;
    using ::inotify_add_watch;
    using ::inotify_rm_watch;
    #endif
}
