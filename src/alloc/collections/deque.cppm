/**
 * @file deque.cppm
 * @module alloc:collections.deque
 * @brief Module file for standard library deque operations.
 *
 * This file contains the implementation of the deque operations in the standard library.
 */

module;

#include <deque>

export module alloc:collections.deque;

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
    using Deque = std::deque<T, Alloc>;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename T>
        using Deque = std::pmr::deque<T>;
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
