/**
 * @file tree_map.cppm
 * @module std:collections.tree_map
 * @brief Module file for standard library tree map operations.
 *
 * This file contains the implementation of the tree map operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.tree_map;
#else
export module stdlib:collections.tree_map;
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
    using alloc::collections::TreeMap;
    using alloc::collections::TreeMultimap;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::collections::Map;
    using alloc::collections::Multimap;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::TreeMap;
        using alloc::collections::pmr::TreeMultimap;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::collections::pmr::Map;
        using alloc::collections::pmr::Multimap;
        #endif
    }

    using alloc::collections::erase_if;

    using alloc::collections::operator==;
    using alloc::collections::operator<=>;

    using alloc::collections::begin;
    using alloc::collections::cbegin;
    using alloc::collections::end;
    using alloc::collections::cend;
    using alloc::collections::rbegin;
    using alloc::collections::crbegin;
    using alloc::collections::rend;
    using alloc::collections::crend;
    using alloc::collections::size;
    using alloc::collections::ssize;
    using alloc::collections::empty;
    using alloc::collections::data;

    using alloc::collections::swap;
}
