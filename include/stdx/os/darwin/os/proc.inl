#pragma once

/**
 * @namespace stdx::os::darwin::os
 * @brief Per-process resource queries - <os/proc.h>.
 */
export namespace stdx::os::darwin::os {
    #if defined(__APPLE__) && __has_include(<os/proc.h>)
    using ::os_proc_available_memory;
    #endif
}
