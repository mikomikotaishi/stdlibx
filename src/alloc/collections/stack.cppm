/**
 * @file stack.cppm
 * @module alloc:collections.stack
 * @brief Module file for standard library stack operations.
 *
 * This file contains the implementation of the stack operations in the standard library.
 */

module;

#include <stack>

export module alloc:collections.stack;

import core;

import :collections.deque;

using core::meta::IsSameValue;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    template <typename T, typename Container = Deque<T>>
        requires IsSameValue<T, typename Container::value_type>
    using Stack = std::stack<T, Container>;

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
    using std::swap;
}
