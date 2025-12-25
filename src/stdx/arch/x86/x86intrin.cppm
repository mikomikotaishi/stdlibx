/**
 * @file x86intrin.cppm
 * @module stdx:arch.x86.x86intrin
 * @brief Module file for x86intrin operations
 *
 * This file contains the implementation for the x86intrin.h header
 */

module;

#if defined(__i386__) && defined(__x86_64__)
#include <x86intrin.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch.x86.x86intrin;
#else
export module stdlibx:arch.x86.x86intrin;
#endif

