/**
 * @file x86.cppm
 * @module stdx:arch.x86
 * @brief Module file for x86 operations.
 *
 * This file contains the implementation of all x86 header modules
 */

module;

#if defined(__i386__) && defined(__x86_64__)
#include <immintrin.h>
#include <x86intrin.h>
#endif

export module stdx:arch.x86;

