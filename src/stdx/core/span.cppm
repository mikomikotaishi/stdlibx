/**
 * @file span.cppm
 * @module stdx:core.span
 * @brief Module file for standard library span operations.
 *
 * This file contains the implementation of the span operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.span;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::DYNAMIC_EXTENT;
    using ::core::Span;

    using ::core::as_bytes;
    using ::core::as_writable_bytes;
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
}
