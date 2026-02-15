/**
 * @file intrin.cppm
 * @module stdx:os.win32.intrin
 * @brief Module file for Win32 operations.
 *
 * This file contains the implementation of the Windows operations from the Windows API libraries,
 * located in <intrin.h>.
 */

module;

#ifdef _WIN32
#include <intrin.h>
#endif

export module stdx:os.win32.intrin;

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
