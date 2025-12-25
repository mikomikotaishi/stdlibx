/**
 * @file random.cppm
 * @module std:random
 * @brief Module file for standard library pseudorandom number generation operations.
 *
 * This file contains the imports for the pseudorandom number generation operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:random;
#else
export module stdlib:random;
#endif

export import :random.random;
