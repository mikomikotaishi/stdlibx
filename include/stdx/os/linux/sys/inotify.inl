#pragma once

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
