/**
 * @file string.cppm
 * @module std:text.string
 * @brief Module file for standard library string operations.
 *
 * This file contains the imports for the string operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text.string;
#else
export module stdlib:text.string;
#endif

export import :text.string.cstring;
export import :text.string.string;
export import :text.string.string_view;
