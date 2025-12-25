/**
 * @file source_location.cppm
 * @module core:meta.source_location
 * @brief Module file for standard library source code location operations.
 *
 * This file contains the implementation of the source code location operations in the standard library.
 */

module;

#if __has_include(<source_location>)
#include <source_location>
#endif

export module core:meta.source_location;

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    #if __has_include(<source_location>)
    using SourceLocation = std::source_location;
    #endif
}
