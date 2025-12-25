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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.win32.intrin;
#else
export module stdlibx:os.win32.intrin;
#endif

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::win32 {
#else 
export namespace stdlibx::os::win32 {
#endif
    #ifdef _WIN32
    using ::_mm_pause;
    using ::_ReadWriteBarrier;
    #endif
}
