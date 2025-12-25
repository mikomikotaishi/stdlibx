/**
 * @file typeinfo.cppm
 * @module std:meta.typeinfo
 * @brief Module file for standard library type information operations.
 *
 * This file contains the implementation of the type information operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:meta.typeinfo;
#else
export module stdlib:meta.typeinfo;
#endif

import core;

/**
 * @namespace std::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdlib::meta {
    using core::meta::TypeInfo;

    using core::meta::BadCastException;
    using core::meta::BadTypeIDException;
}
