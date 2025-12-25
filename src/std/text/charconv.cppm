/**
 * @file charconv.cppm
 * @module std:text.charconv
 * @brief Module file for standard library character conversion operations.
 *
 * This file contains the imports for the character conversion operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text.charconv;
#else
export module stdlib:text.charconv;
#endif

import core;

/**
 * @namespace std::text
 * @brief Wrapper namespace for standard library text operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::text {
#else 
export namespace stdlib::text {
#endif
    using core::text::CharsFormat;
    using core::text::FromCharsResult;
    using core::text::ToCharsResult;
    using core::text::from_chars;
    using core::text::to_chars;
}
