/**
 * @file initializer_list.cppm
 * @module std:core.initializer_list
 * @brief Module file for importing the InitializerList class from the standard library.
 * 
 * This file exports the InitializerList class operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.initializer_list;
#else
export module stdlib:core.initializer_list;
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
    using ::core::InitializerList;

    using ::core::begin;
    using ::core::end;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
