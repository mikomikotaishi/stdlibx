/**
 * @file charconv.cppm
 * @module std:fmt.charconv
 * @brief Module file for standard library character conversion operations.
 *
 * This file contains the implementation of the character conversion operations in the standard library.
 */

module;

#include <charconv>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fmt.charconv;
#else
export module stdlib:fmt.charconv;
#endif

/**
 * @namespace std::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::fmt {
#else 
export namespace stdlib::fmt {
#endif
    using CharsFormat = std::chars_format;
    using FromCharsResult = std::from_chars_result;
    using ToCharsResult = std::to_chars_result;

    using std::from_chars;
    using std::to_chars;
}
