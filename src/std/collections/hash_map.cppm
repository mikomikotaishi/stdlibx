/**
 * @file hash_map.cppm
 * @module std:collections.hash_map
 * @brief Module file for standard library hash map operations.
 *
 * This file contains the implementation of the hash map operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.hash_map;
#else
export module stdlib:collections.hash_map;
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
    using alloc::collections::HashMap;
    using alloc::collections::HashMultimap;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::collections::UnorderedMap;
    using alloc::collections::UnorderedMultimap;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::HashMap;
        using alloc::collections::pmr::HashMultimap;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::collections::pmr::UnorderedMap;
        using alloc::collections::pmr::UnorderedMultimap;
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

    using alloc::collections::hash;
    using alloc::collections::swap;
}
