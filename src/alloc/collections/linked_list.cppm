/**
 * @file linked_list.cppm
 * @module alloc:collections.linked_list
 * @brief Module file for standard library linked list operations.
 *
 * This file contains the implementation of the linked list operations in the standard library.
 */

module;

#include <list>

export module alloc:collections.linked_list;

import core;

import :mem.memory;

using core::meta::IsSameValue;
using core::meta::RemoveConstVolatile;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename T, typename Alloc = Allocator<T>>
        requires 
            IsSameValue<typename RemoveConstVolatile<T>::type, T> &&
            IsSameValue<typename Alloc::value_type, T>
    using LinkedList = std::list<T, Alloc>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T, typename Alloc = Allocator<T>>
    using List = LinkedList<T, Alloc>;
    #endif

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename T>
        using LinkedList = std::pmr::list<T>;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        template <typename T>
        using List = LinkedList<T>;
        #endif
    }

    using std::erase;
    using std::erase_if;

    using std::operator==;
    using std::operator<=>;

    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::empty;
    using std::data;
    
    using std::swap;
}
