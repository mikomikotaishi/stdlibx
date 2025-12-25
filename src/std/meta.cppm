/**
 * @file meta.cppm
 * @module std:meta
 * @brief Module file for standard library metaprogramming operations.
 *
 * This file contains the imports for the metaprogramming operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:meta;
#else
export module stdlib:meta;
#endif

export import :meta.source_location;
export import :meta.stacktrace;
export import :meta.type_traits;
export import :meta.typeindex;
export import :meta.typeinfo;
