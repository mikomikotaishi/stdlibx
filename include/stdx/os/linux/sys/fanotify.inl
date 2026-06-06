#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/fanotify.h>)
    using ::fanotify_init;
    using ::fanotify_mark;
    #endif
}
