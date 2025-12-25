/**
 * @file vector.cppm
 * @module alloc:collections.vector
 * @brief Module file for standard library vector operations.
 *
 * This file contains the implementation of the vector operations in the standard library.
 */

module;

#include <vector>

export module alloc:collections.vector;

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
    using Vector = std::vector<T>;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        template <typename T>
        using Vector = std::pmr::vector<T>;
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

    using std::hash;
    using std::formatter;
    using std::swap;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
