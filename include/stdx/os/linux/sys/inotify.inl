#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/inotify.h>)
    using INotifyEvent = ::inotify_event;

    using ::inotify_init;
    using ::inotify_init1;
    using ::inotify_add_watch;
    using ::inotify_rm_watch;
    #endif
}
