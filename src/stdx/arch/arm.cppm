/**
 * @file arm.cppm
 * @module stdx:arch.arm
 * @brief Module file for arm operations.
 *
 * This file contains the implementation of all arm header modules
 */

module;

#ifdef __ARM_FP
#include <arm_neon.h>
#endif

export module stdx:arch.arm;

