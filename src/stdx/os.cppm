/**
 * @file os.cppm
 * @module stdx:os
 * @brief Module file for operating system operations.
 *
 * This file contains the imports for the operating system operations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os;
#else
export module stdlibx:os;
#endif

export import :os.linux;
export import :os.unix;
export import :os.win32;
