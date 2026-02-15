/**
 * @file vector.cppm
 * @module stdx:collections.vector
 * @brief Module file for standard library vector operations.
 *
 * This file contains the implementation of the vector operations in the standard library.
 */

module;

export module stdx:collections.vector;

import alloc;

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using alloc::collections::Vector;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::collections::pmr::Vector;
    }

    using alloc::collections::erase;
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
    using alloc::collections::formatter;
    using alloc::collections::swap;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
