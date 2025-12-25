/**
 * @file sequence.cppm
 * @module std:core.sequence
 * @brief Module file for standard library sequence operations.
 *
 * This file contains the implementation of the sequence operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.sequence;
#else
export module stdlib:core.sequence;
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
    using ::core::IntegerSequence;
    using ::core::make_integer_sequence;

    using ::core::IndexSequence;
    using ::core::IndexSequenceFor;
    using ::core::make_index_sequence;

    using ::core::begin;
    using ::core::end;
    using ::core::swap;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
