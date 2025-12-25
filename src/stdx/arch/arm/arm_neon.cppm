/**
 * @file arm_neon.cppm
 * @module stdx:arch.arm.arm_neon
 * @brief Module file for arm_neon operations
 *
 * This file contains the implementation for the arm_neon.h header
 */

module;

#ifdef __ARM_FP
#include <arm_neon.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch.arm.arm_neon;
#else
export module stdlibx:arch.arm.arm_neon;
#endif
