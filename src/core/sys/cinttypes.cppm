/**
 * @file cinttypes.cppm
 * @module core:sys.cinttypes
 * @brief Module file for constants of the standard library C-style integer formatting constant operations.
 *
 * This file contains the implementation of the constants of the C-style integer formatting constant operations in the standard library.
 */

export module core:sys.cinttypes;

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    // Print format specifiers for 32-bit integers
    inline constexpr char PRId32[] = "d"; ///< Format specifier for printing int32_t as decimal
    inline constexpr char PRIdLEAST32[] = "d"; ///< Format specifier for printing int_least32_t as decimal
    inline constexpr char PRIdFAST32[] = "d"; ///< Format specifier for printing int_fast32_t as decimal
    inline constexpr char PRIi32[] = "i"; ///< Format specifier for printing int32_t as decimal with base indicator
    inline constexpr char PRIiLEAST32[] = "i"; ///< Format specifier for printing int_least32_t with base indicator
    inline constexpr char PRIiFAST32[] = "i"; ///< Format specifier for printing int_fast32_t with base indicator
    inline constexpr char PRIo32[] = "o"; ///< Format specifier for printing uint32_t as octal
    inline constexpr char PRIoLEAST32[] = "o"; ///< Format specifier for printing uint_least32_t as octal
    inline constexpr char PRIoFAST32[] = "o"; ///< Format specifier for printing uint_fast32_t as octal
    inline constexpr char PRIu32[] = "u"; ///< Format specifier for printing uint32_t as unsigned decimal
    inline constexpr char PRIuLEAST32[] = "u"; ///< Format specifier for printing uint_least32_t as unsigned decimal
    inline constexpr char PRIuFAST32[] = "u"; ///< Format specifier for printing uint_fast32_t as unsigned decimal
    inline constexpr char PRIx32[] = "x"; ///< Format specifier for printing uint32_t as lowercase hex
    inline constexpr char PRIxLEAST32[] = "x"; ///< Format specifier for printing uint_least32_t as lowercase hex
    inline constexpr char PRIxFAST32[] = "x"; ///< Format specifier for printing uint_fast32_t as lowercase hex
    inline constexpr char PRIX32[] = "X"; ///< Format specifier for printing uint32_t as uppercase hex
    inline constexpr char PRIXLEAST32[] = "X"; ///< Format specifier for printing uint_least32_t as uppercase hex
    inline constexpr char PRIXFAST32[] = "X"; ///< Format specifier for printing uint_fast32_t as uppercase hex
    
    // Scan format specifiers for 32-bit integers
    inline constexpr char SCNd32[] = "d"; ///< Format specifier for scanning int32_t as decimal
    inline constexpr char SCNdLEAST32[] = "d"; ///< Format specifier for scanning int_least32_t as decimal
    inline constexpr char SCNdFAST32[] = "d"; ///< Format specifier for scanning int_fast32_t as decimal
    inline constexpr char SCNi32[] = "i"; ///< Format specifier for scanning int32_t with auto base detection
    inline constexpr char SCNiLEAST32[] = "i"; ///< Format specifier for scanning int_least32_t with auto base detection
    inline constexpr char SCNiFAST32[] = "i"; ///< Format specifier for scanning int_fast32_t with auto base detection
    inline constexpr char SCNo32[] = "o"; ///< Format specifier for scanning uint32_t as octal
    inline constexpr char SCNoLEAST32[] = "o"; ///< Format specifier for scanning uint_least32_t as octal
    inline constexpr char SCNoFAST32[] = "o"; ///< Format specifier for scanning uint_fast32_t as octal
    inline constexpr char SCNu32[] = "u"; ///< Format specifier for scanning uint32_t as unsigned decimal
    inline constexpr char SCNuLEAST32[] = "u"; ///< Format specifier for scanning uint_least32_t as unsigned decimal
    inline constexpr char SCNuFAST32[] = "u"; ///< Format specifier for scanning uint_fast32_t as unsigned decimal
    inline constexpr char SCNx32[] = "x"; ///< Format specifier for scanning uint32_t as hex (both cases)
    inline constexpr char SCNxLEAST32[] = "x"; ///< Format specifier for scanning uint_least32_t as hex (both cases)
    inline constexpr char SCNxFAST32[] = "x"; ///< Format specifier for scanning uint_fast32_t as hex (both cases)
}
