#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/psx_syscall.h>)
    using ::psx_load_syscalls;
    using ::psx_set_sensitivity;
    #endif
}
