/**
 * @file cinttypes.cppm
 * @module std:sys.cinttypes
 * @brief Module file for constants of the standard library C-style integer formatting constant operations.
 *
 * This file contains the implementation of the constants of the C-style integer formatting constant operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sys.cinttypes;
#else
export module stdlib:sys.cinttypes;
#endif

import core;

/**
 * @namespace std::sys
 * @brief Wrapper namespace for standard library system operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::sys {
#else 
export namespace stdlib::sys {
#endif
    // Print format specifiers for 32-bit integers
    using core::sys::PRId32; ///< Format specifier for printing int32_t as decimal
    using core::sys::PRIdLEAST32; ///< Format specifier for printing int_least32_t as decimal
    using core::sys::PRIdFAST32; ///< Format specifier for printing int_fast32_t as decimal
    using core::sys::PRIi32; ///< Format specifier for printing int32_t as decimal with base indicator
    using core::sys::PRIiLEAST32; ///< Format specifier for printing int_least32_t with base indicator
    using core::sys::PRIiFAST32; ///< Format specifier for printing int_fast32_t with base indicator
    using core::sys::PRIo32; ///< Format specifier for printing uint32_t as octal
    using core::sys::PRIoLEAST32; ///< Format specifier for printing uint_least32_t as octal
    using core::sys::PRIoFAST32; ///< Format specifier for printing uint_fast32_t as octal
    using core::sys::PRIu32; ///< Format specifier for printing uint32_t as unsigned decimal
    using core::sys::PRIuLEAST32; ///< Format specifier for printing uint_least32_t as unsigned decimal
    using core::sys::PRIuFAST32; ///< Format specifier for printing uint_fast32_t as unsigned decimal
    using core::sys::PRIx32; ///< Format specifier for printing uint32_t as lowercase hex
    using core::sys::PRIxLEAST32; ///< Format specifier for printing uint_least32_t as lowercase hex
    using core::sys::PRIxFAST32; ///< Format specifier for printing uint_fast32_t as lowercase hex
    using core::sys::PRIX32; ///< Format specifier for printing uint32_t as uppercase hex
    using core::sys::PRIXLEAST32; ///< Format specifier for printing uint_least32_t as uppercase hex
    using core::sys::PRIXFAST32; ///< Format specifier for printing uint_fast32_t as uppercase hex
    
    // Scan format specifiers for 32-bit integers
    using core::sys::SCNd32; ///< Format specifier for scanning int32_t as decimal
    using core::sys::SCNdLEAST32; ///< Format specifier for scanning int_least32_t as decimal
    using core::sys::SCNdFAST32; ///< Format specifier for scanning int_fast32_t as decimal
    using core::sys::SCNi32; ///< Format specifier for scanning int32_t with auto base detection
    using core::sys::SCNiLEAST32; ///< Format specifier for scanning int_least32_t with auto base detection
    using core::sys::SCNiFAST32; ///< Format specifier for scanning int_fast32_t with auto base detection
    using core::sys::SCNo32; ///< Format specifier for scanning uint32_t as octal
    using core::sys::SCNoLEAST32; ///< Format specifier for scanning uint_least32_t as octal
    using core::sys::SCNoFAST32; ///< Format specifier for scanning uint_fast32_t as octal
    using core::sys::SCNu32; ///< Format specifier for scanning uint32_t as unsigned decimal
    using core::sys::SCNuLEAST32; ///< Format specifier for scanning uint_least32_t as unsigned decimal
    using core::sys::SCNuFAST32; ///< Format specifier for scanning uint_fast32_t as unsigned decimal
    using core::sys::SCNx32; ///< Format specifier for scanning uint32_t as hex (both cases)
    using core::sys::SCNxLEAST32; ///< Format specifier for scanning uint_least32_t as hex (both cases)
    using core::sys::SCNxFAST32; ///< Format specifier for scanning uint_fast32_t as hex (both cases)
}
