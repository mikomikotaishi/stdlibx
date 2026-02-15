/**
 * @file array.cppm
 * @module stdx:core.array
 * @brief Module file for standard library array operations.
 *
 * This file contains the implementation of the array operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.array;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Array;

    using ::core::operator==;
    using ::core::operator<=>;
    using ::core::to_array;
    using ::core::get;
    
    using ::core::begin;
    using ::core::cbegin;
    using ::core::end;
    using ::core::cend;
    using ::core::rbegin;
    using ::core::crbegin;
    using ::core::rend;
    using ::core::crend;
    using ::core::size;
    using ::core::ssize;
    using ::core::empty;
    using ::core::data;

    using ::core::swap;
}
