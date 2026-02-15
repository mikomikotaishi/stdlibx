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

export module stdx:os.linux.sys.inotify;

export import :os.linux.sys.inotify.constants;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using INotifyEvent = ::inotify_event;

    using ::inotify_init;
    using ::inotify_init1;
    using ::inotify_add_watch;
    using ::inotify_rm_watch;
    #endif
}
