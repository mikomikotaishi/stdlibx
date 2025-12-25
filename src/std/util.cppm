/**
 * @file util.cppm
 * @module std:util
 * @brief Module file for standard library utility operations.
 *
 * This file contains the imports for the utility operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:util;
#else
export module stdlib:util;
#endif

export import :util.algorithm;
export import :util.cstdlib;
export import :util.numeric;
export import :util.utility;
