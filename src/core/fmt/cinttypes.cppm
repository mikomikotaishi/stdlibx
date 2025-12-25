/**
 * @file cinttypes.cppm
 * @module core:fmt.cinttypes
 * @brief Module file for standard library C-style integer formatting constant operations.
 *
 * This file contains the implementation of the C-style integer formatting constants operations in the standard library.
 */

module;

#include <cinttypes>

export module core:fmt.cinttypes;

export import :fmt.cinttypes.constants;

/**
 * @namespace core::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
export namespace core::fmt {
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using IMaxDiv_t = std::imaxdiv_t;
    using IntegerMaxDiv_t = std::imaxdiv_t;
    using IMaxDiv = std::imaxdiv_t;
    using IntegerMaxDiv = std::imaxdiv_t;
    using IntegerMaximumDivision_t = std::imaxdiv_t;
    #endif

    using IntegerMaximumDivision = std::imaxdiv_t;

    using std::imaxabs;
    using std::imaxdiv;
    using std::strtoimax;
    using std::strtoumax;
    using std::wcstoimax;
    using std::wcstoumax;
}
