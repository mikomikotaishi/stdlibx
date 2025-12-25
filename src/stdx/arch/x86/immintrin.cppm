/**
 * @file immintrin.cppm
 * @module stdx:arch.x86.immintrin
 * @brief Module file for immintrin operations
 *
 * This file contains the implementation for the immintrin.h header
 */

module;

#if defined(__i386__) && defined(__x86_64__)
#include <immintrin.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch.x86.immintrin;
#else
export module stdlibx:arch.x86.immintrin;
#endif

