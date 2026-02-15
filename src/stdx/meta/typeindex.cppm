/**
 * @file typeindex.cppm
 * @module stdx:meta.typeindex
 * @brief Module file for standard library type index operations.
 *
 * This file contains the implementation of the type index operations in the standard library.
 */

module;

export module stdx:meta.typeindex;

import core;

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    using core::meta::TypeIndex;

    using core::meta::hash;
}
