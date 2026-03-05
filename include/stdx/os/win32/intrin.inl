#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    using ::_mm_pause;
    using ::_ReadWriteBarrier;
    #endif
}
