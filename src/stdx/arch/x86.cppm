/**
 * @file x86.cppm
 * @module stdx:arch.x86
 * @brief Module file for x86 operations.
 *
 * This file contains the implementation of all x86 header modules
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch.x86;
#else
export module stdlibx:arch.x86;
#endif

export import :arch.x86.immintrin;
export import :arch.x86.x86intrin;
