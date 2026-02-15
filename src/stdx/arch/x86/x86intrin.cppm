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

export module stdx:arch.x86.x86intrin;
