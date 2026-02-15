/**
 * @file source_location.cppm
 * @module stdx:meta.source_location
 * @brief Module file for standard library source code location operations.
 *
 * This file contains the implementation of the source code location operations in the standard library.
 */

module;

export module stdx:meta.source_location;

import core;

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    #if __has_include(<source_location>)
    using core::meta::SourceLocation;
    #endif
}
