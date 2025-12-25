/**
 * @file text.cppm
 * @module std:text
 * @brief Module file for standard library text manipulation operations.
 *
 * This file contains the imports for the text manipulation operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text;
#else
export module stdlib:text;
#endif

export import :text.cctype;
export import :text.charconv;
export import :text.cstdlib;
export import :text.regex;
export import :text.string;
