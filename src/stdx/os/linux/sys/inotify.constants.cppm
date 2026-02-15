/**
 * @file inotify.constants.cppm
 * @module stdx:os.linux.sys.inotify.constants
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the constants used 
 * in <sys/inotify.h>.
 */

module;

export module stdx:os.linux.sys.inotify.constants;

/**
 * @namespace stdx::os::inotify
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::inotify {
    inline constexpr int IN_CLOEXEC = 02000000;
    inline constexpr int IN_NONBLOCK = 00004000;
}
