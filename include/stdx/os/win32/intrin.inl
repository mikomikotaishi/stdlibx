#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Windows API operations.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<intrin.h>)
    using ::_mm_pause;
    using ::_ReadWriteBarrier;
    #endif
}
