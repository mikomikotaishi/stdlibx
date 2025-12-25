/**
 * @internal
 * @file cinttypes.constants.cppm
 * @module core:fmt.cinttypes.constants
 * @brief Module file for constants of the standard library C-style integer formatting constant operations.
 *
 * This file contains the implementation of the constants of the C-style integer formatting constant operations in the standard library.
 */

export module core:fmt.cinttypes.constants;

/**
 * @namespace core::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
export namespace core::fmt {
    // Print format specifiers for 32-bit integers
    constexpr char PRId32[] = "d"; ///< Format specifier for printing int32_t as decimal
    constexpr char PRIdLEAST32[] = "d"; ///< Format specifier for printing int_least32_t as decimal
    constexpr char PRIdFAST32[] = "d"; ///< Format specifier for printing int_fast32_t as decimal
    constexpr char PRIi32[] = "i"; ///< Format specifier for printing int32_t as decimal with base indicator
    constexpr char PRIiLEAST32[] = "i"; ///< Format specifier for printing int_least32_t with base indicator
    constexpr char PRIiFAST32[] = "i"; ///< Format specifier for printing int_fast32_t with base indicator
    constexpr char PRIo32[] = "o"; ///< Format specifier for printing uint32_t as octal
    constexpr char PRIoLEAST32[] = "o"; ///< Format specifier for printing uint_least32_t as octal
    constexpr char PRIoFAST32[] = "o"; ///< Format specifier for printing uint_fast32_t as octal
    constexpr char PRIu32[] = "u"; ///< Format specifier for printing uint32_t as unsigned decimal
    constexpr char PRIuLEAST32[] = "u"; ///< Format specifier for printing uint_least32_t as unsigned decimal
    constexpr char PRIuFAST32[] = "u"; ///< Format specifier for printing uint_fast32_t as unsigned decimal
    constexpr char PRIx32[] = "x"; ///< Format specifier for printing uint32_t as lowercase hex
    constexpr char PRIxLEAST32[] = "x"; ///< Format specifier for printing uint_least32_t as lowercase hex
    constexpr char PRIxFAST32[] = "x"; ///< Format specifier for printing uint_fast32_t as lowercase hex
    constexpr char PRIX32[] = "X"; ///< Format specifier for printing uint32_t as uppercase hex
    constexpr char PRIXLEAST32[] = "X"; ///< Format specifier for printing uint_least32_t as uppercase hex
    constexpr char PRIXFAST32[] = "X"; ///< Format specifier for printing uint_fast32_t as uppercase hex
    
    // Scan format specifiers for 32-bit integers
    constexpr char SCNd32[] = "d"; ///< Format specifier for scanning int32_t as decimal
    constexpr char SCNdLEAST32[] = "d"; ///< Format specifier for scanning int_least32_t as decimal
    constexpr char SCNdFAST32[] = "d"; ///< Format specifier for scanning int_fast32_t as decimal
    constexpr char SCNi32[] = "i"; ///< Format specifier for scanning int32_t with auto base detection
    constexpr char SCNiLEAST32[] = "i"; ///< Format specifier for scanning int_least32_t with auto base detection
    constexpr char SCNiFAST32[] = "i"; ///< Format specifier for scanning int_fast32_t with auto base detection
    constexpr char SCNo32[] = "o"; ///< Format specifier for scanning uint32_t as octal
    constexpr char SCNoLEAST32[] = "o"; ///< Format specifier for scanning uint_least32_t as octal
    constexpr char SCNoFAST32[] = "o"; ///< Format specifier for scanning uint_fast32_t as octal
    constexpr char SCNu32[] = "u"; ///< Format specifier for scanning uint32_t as unsigned decimal
    constexpr char SCNuLEAST32[] = "u"; ///< Format specifier for scanning uint_least32_t as unsigned decimal
    constexpr char SCNuFAST32[] = "u"; ///< Format specifier for scanning uint_fast32_t as unsigned decimal
    constexpr char SCNx32[] = "x"; ///< Format specifier for scanning uint32_t as hex (both cases)
    constexpr char SCNxLEAST32[] = "x"; ///< Format specifier for scanning uint_least32_t as hex (both cases)
    constexpr char SCNxFAST32[] = "x"; ///< Format specifier for scanning uint_fast32_t as hex (both cases)
}
