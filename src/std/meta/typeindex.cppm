/**
 * @file typeindex.cppm
 * @module std:meta.typeindex
 * @brief Module file for standard library type index operations.
 *
 * This file contains the implementation of the type index operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:meta.typeindex;
#else
export module stdlib:meta.typeindex;
#endif

import core;

/**
 * @namespace std::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::meta {
#else 
export namespace stdlib::meta {
#endif
    using core::meta::TypeIndex;

    using core::meta::hash;
}
