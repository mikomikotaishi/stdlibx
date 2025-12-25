/**
 * @file span.cppm
 * @module std:core.span
 * @brief Module file for standard library span operations.
 *
 * This file contains the implementation of the span operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.span;
#else
export module stdlib:core.span;
#endif

import core;

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
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

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
