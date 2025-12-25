/**
 * @file win32.cppm
 * @module stdx:os.win32
 * @brief Module file for Win32 operations.
 *
 * This file contains the implementation of the Windows operations from the Windows API libraries.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.win32;
#else
export module stdlibx:os.win32;
#endif

export import :os.win32.intrin;
export import :os.win32.windows;
