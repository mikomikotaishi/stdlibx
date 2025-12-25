/**
 * @file bitset.cppm
 * @module std:collections.bitset
 * @brief Module file for standard library bitset operations.
 *
 * This file contains the implementation of the bitset operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.bitset;
#else
export module stdlib:collections.bitset;
#endif

import core;

/**
 * @namespace std::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::collections {
#else 
export namespace stdlib::collections {
#endif
    using core::collections::BitSet;

    using core::collections::operator&;
    using core::collections::operator|;
    using core::collections::operator^;
    using core::collections::operator<<;
    using core::collections::operator>>;

    using core::collections::hash;
    using core::collections::swap;
}
