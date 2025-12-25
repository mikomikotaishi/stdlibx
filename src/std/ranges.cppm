/**
 * @file ranges.cppm
 * @module std:ranges
 * @brief Module file for standard library ranges operations.
 *
 * This file contains the imports for the ranges operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:ranges;
#else
export module stdlib:ranges;
#endif

export import :ranges.algorithm;
export import :ranges.functional;
export import :ranges.generator;
export import :ranges.iterator;
export import :ranges.memory;
export import :ranges.ranges;
