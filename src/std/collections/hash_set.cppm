/**
 * @file hash_set.cppm
 * @module std:collections.hash_set
 * @brief Module file for standard library hash set operations.
 *
 * This file contains the implementation of the hash set operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections.hash_set;
#else
export module stdlib:collections.hash_set;
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
    using alloc::collections::HashSet;
    using alloc::collections::HashMultiset;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::collections::UnorderedSet;
    using alloc::collections::UnorderedMultiset;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::HashSet;
        using alloc::collections::pmr::HashMultiset;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::collections::pmr::UnorderedSet;
        using alloc::collections::pmr::UnorderedMultiset;
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
