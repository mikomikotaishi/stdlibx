/**
 * @file fmt.cppm
 * @module std:fmt
 * @brief Module file for standard library format and regex operations.
 *
 * This file contains the imports for the text formatting operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fmt;
#else
export module stdlib:fmt;
#endif

export import :fmt.charconv;
export import :fmt.cinttypes;
export import :fmt.format;
