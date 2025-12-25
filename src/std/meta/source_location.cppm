/**
 * @file source_location.cppm
 * @module std:meta.source_location
 * @brief Module file for standard library source code location operations.
 *
 * This file contains the implementation of the source code location operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:meta.source_location;
#else
export module stdlib:meta.source_location;
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
    #if __has_include(<source_location>)
    using core::meta::SourceLocation;
    #endif
}
