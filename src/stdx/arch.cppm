/**
 * @file arch.cppm
 * @module stdx:arch
 * @brief Module file for architecture-specific intrinsic operations.
 *
 * This file contains the imports for the architecture-specific intrinsic operations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:arch;
#else
export module stdlibx:arch;
#endif

export import :arch.arm;
export import :arch.x86;
