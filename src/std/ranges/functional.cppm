/**
 * @file functional.cppm
 * @module std:ranges.functional
 * @brief Module file for standard library functional ranges operations.
 *
 * This file contains the implementation of the functional ranges operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:ranges.functional;
#else
export module stdlib:ranges.functional;
#endif

import core;

/**
 * @namespace std::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::ranges {
#else 
export namespace stdlib::ranges {
#endif
    using core::ranges::equal_to;
    using core::ranges::not_equal_to;
    using core::ranges::greater;
    using core::ranges::less;
    using core::ranges::greater_equal;
    using core::ranges::less_equal;
}
