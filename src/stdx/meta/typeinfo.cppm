/**
 * @file typeinfo.cppm
 * @module stdx:meta.typeinfo
 * @brief Module file for standard library type information operations.
 *
 * This file contains the implementation of the type information operations in the standard library.
 */

module;

export module stdx:meta.typeinfo;

import core;

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    using core::meta::TypeInfo;

    using core::meta::BadCastException;
    using core::meta::BadTypeIDException;
}
