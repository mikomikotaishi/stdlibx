/**
 * @file typeinfo.cppm
 * @module core:meta.typeinfo
 * @brief Module file for standard library type information operations.
 *
 * This file contains the implementation of the type information operations in the standard library.
 */

module;

#include <typeinfo>

export module core:meta.typeinfo;

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    using TypeInfo = std::type_info;

    #ifndef STDLIBX_NO_STD
    using BadCastException = std::bad_cast;
    using BadTypeIDException = std::bad_typeid;
    #endif
}
