/**
 * @file variant.cppm
 * @module std:core.variant
 * @brief Module file for importing the Variant class from the standard library.
 * 
 * This file exports the Variant class operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.variant;
#else
export module stdlib:core.variant;
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
    using ::core::Variant;
    using ::core::Monostate;
    using ::core::VariantSize;
    using ::core::VariantAlternative;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::VariantAlternative_t;
    #endif
    using ::core::VariantAlternativeType;
    using ::core::BadVariantAccessException;

    using ::core::hash;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::VariantNPos;
    #endif

    using ::core::VariantNoPosition;

    using ::core::visit;
    using ::core::holds_alternative;
    using ::core::get;
    using ::core::get_if;
    using ::core::swap;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
