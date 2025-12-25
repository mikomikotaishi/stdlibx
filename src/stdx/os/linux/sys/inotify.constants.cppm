/**
 * @file inotify.constants.cppm
 * @module stdx:os.linux.sys.inotify.constants
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the constants used 
 * in <sys/inotify.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.inotify.constants;
#else
export module stdlibx:os.linux.sys.inotify.constants;
#endif

/**
 * @namespace stdx::os::inotify
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::inotify {
#else 
export namespace stdlibx::os::inotify {
#endif
    inline constexpr int IN_CLOEXEC = 02000000;
    inline constexpr int IN_NONBLOCK = 00004000;
}
