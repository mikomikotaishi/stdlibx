/**
 * @file flat_map.cppm
 * @module std:collections.flat_map
 * @brief Module file for standard library flat map operations.
 *
 * This file contains the implementation of the flat map operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.flat_map;
#else
export module stdlib:collections.flat_map;
#endif

import alloc;

/**
 * @namespace std::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::collections {
#else 
export namespace stdlib::collections {
#endif
    #if __has_include(<flat_map>)
    using alloc::collections::FlatMap;
    using alloc::collections::FlatMultimap;

    using alloc::collections::SortedUniqueTag;
    using alloc::collections::SortedUnique;
    using alloc::collections::SortedEquivalentTag;
    using alloc::collections::SortedEquivalent;

    using alloc::collections::erase_if;
    #endif
}
