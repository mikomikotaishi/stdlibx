/**
 * @file fanotify.cppm
 * @module stdx:os.linux.sys.fanotify
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/fanotify.h>.
 */

module;

#ifdef __linux__
#include <sys/fanotify.h>
#endif

export module stdx:os.linux.sys.fanotify;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::fanotify_init;
    using ::fanotify_mark;
    #endif
}
