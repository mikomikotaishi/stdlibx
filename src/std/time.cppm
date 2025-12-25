/**
 * @file time.cppm
 * @module std:time
 * @brief Module file for standard library time operations.
 *
 * This file contains the imports for the time operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:time;
#else
export module stdlib:time;
#endif

export import :time.chrono;
export import :time.ctime;
