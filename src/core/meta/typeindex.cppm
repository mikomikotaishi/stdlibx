/**
 * @file typeindex.cppm
 * @module core:meta.typeindex
 * @brief Module file for standard library type index operations.
 *
 * This file contains the implementation of the type index operations in the standard library.
 */

module;

#include <typeindex>

export module core:meta.typeindex;

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    using TypeIndex = std::type_index;

    using std::hash;
}
