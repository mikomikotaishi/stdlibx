/**
 * @file arm.cppm
 * @module stdx:arch.arm
 * @brief Module file for arm operations.
 *
 * This file contains the implementation of all arm header modules
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch.arm;
#else
export module stdlibx:arch.arm;
#endif

export import :arch.arm.arm_neon;
