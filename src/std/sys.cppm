/**
 * @file sys.cppm
 * @module std:sys
 * @brief Module file for standard library system operations.
 *
 * This file contains the imports for the system operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sys;
#else
export module stdlib:sys;
#endif

export import :sys.csetjmp;
export import :sys.csignal;
export import :sys.cstdlib;
export import :sys.utility;
