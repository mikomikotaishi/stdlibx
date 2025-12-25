/**
 * @file iter.cppm
 * @module std:iter
 * @brief Module file for standard library iterator operations.
 *
 * This file contains the imports for the iterator operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:iter;
#else
export module stdlib:iter;
#endif

export import :iter.iterator;
