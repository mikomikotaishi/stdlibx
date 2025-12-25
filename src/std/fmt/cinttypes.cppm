/**
 * @file cinttypes.cppm
 * @module std:fmt.cinttypes
 * @brief Module file for standard library C-style integer formatting constant operations.
 *
 * This file contains the implementation of the C-style integer formatting constants operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fmt.cinttypes;
#else
export module stdlib:fmt.cinttypes;
#endif

import core;

/**
 * @namespace std::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::fmt {
#else 
export namespace stdlib::fmt {
#endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::fmt::IMaxDiv_t;
    using core::fmt::IntegerMaxDiv_t;
    using core::fmt::IMaxDiv;
    using core::fmt::IntegerMaxDiv;
    using core::fmt::IntegerMaximumDivision_t;
    #endif

    using core::fmt::IntegerMaximumDivision;

    using core::fmt::imaxabs;
    using core::fmt::imaxdiv;
    using core::fmt::strtoimax;
    using core::fmt::strtoumax;
    using core::fmt::wcstoimax;
    using core::fmt::wcstoumax;

    // Print format specifiers for 32-bit integers
    using core::fmt::PRId32; ///< Format specifier for printing int32_t as decimal
    using core::fmt::PRIdLEAST32; ///< Format specifier for printing int_least32_t as decimal
    using core::fmt::PRIdFAST32; ///< Format specifier for printing int_fast32_t as decimal
    using core::fmt::PRIi32; ///< Format specifier for printing int32_t as decimal with base indicator
    using core::fmt::PRIiLEAST32; ///< Format specifier for printing int_least32_t with base indicator
    using core::fmt::PRIiFAST32; ///< Format specifier for printing int_fast32_t with base indicator
    using core::fmt::PRIo32; ///< Format specifier for printing uint32_t as octal
    using core::fmt::PRIoLEAST32; ///< Format specifier for printing uint_least32_t as octal
    using core::fmt::PRIoFAST32; ///< Format specifier for printing uint_fast32_t as octal
    using core::fmt::PRIu32; ///< Format specifier for printing uint32_t as unsigned decimal
    using core::fmt::PRIuLEAST32; ///< Format specifier for printing uint_least32_t as unsigned decimal
    using core::fmt::PRIuFAST32; ///< Format specifier for printing uint_fast32_t as unsigned decimal
    using core::fmt::PRIx32; ///< Format specifier for printing uint32_t as lowercase hex
    using core::fmt::PRIxLEAST32; ///< Format specifier for printing uint_least32_t as lowercase hex
    using core::fmt::PRIxFAST32; ///< Format specifier for printing uint_fast32_t as lowercase hex
    using core::fmt::PRIX32; ///< Format specifier for printing uint32_t as uppercase hex
    using core::fmt::PRIXLEAST32; ///< Format specifier for printing uint_least32_t as uppercase hex
    using core::fmt::PRIXFAST32; ///< Format specifier for printing uint_fast32_t as uppercase hex
    
    // Scan format specifiers for 32-bit integers
    using core::fmt::SCNd32; ///< Format specifier for scanning int32_t as decimal
    using core::fmt::SCNdLEAST32; ///< Format specifier for scanning int_least32_t as decimal
    using core::fmt::SCNdFAST32; ///< Format specifier for scanning int_fast32_t as decimal
    using core::fmt::SCNi32; ///< Format specifier for scanning int32_t with auto base detection
    using core::fmt::SCNiLEAST32; ///< Format specifier for scanning int_least32_t with auto base detection
    using core::fmt::SCNiFAST32; ///< Format specifier for scanning int_fast32_t with auto base detection
    using core::fmt::SCNo32; ///< Format specifier for scanning uint32_t as octal
    using core::fmt::SCNoLEAST32; ///< Format specifier for scanning uint_least32_t as octal
    using core::fmt::SCNoFAST32; ///< Format specifier for scanning uint_fast32_t as octal
    using core::fmt::SCNu32; ///< Format specifier for scanning uint32_t as unsigned decimal
    using core::fmt::SCNuLEAST32; ///< Format specifier for scanning uint_least32_t as unsigned decimal
    using core::fmt::SCNuFAST32; ///< Format specifier for scanning uint_fast32_t as unsigned decimal
    using core::fmt::SCNx32; ///< Format specifier for scanning uint32_t as hex (both cases)
    using core::fmt::SCNxLEAST32; ///< Format specifier for scanning uint_least32_t as hex (both cases)
    using core::fmt::SCNxFAST32; ///< Format specifier for scanning uint_fast32_t as hex (both cases)
}
