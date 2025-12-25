/**
 * @file any.cppm
 * @module std:core.any
 * @brief Module file for importing the Any class from the standard library.
 * 
 * This file exports the Any class operations in the standard library.
 */

module;

#include <any>

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.any;
#else
export module stdlib:core.any;
#endif

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    using Any = std::any;

    using BadAnyCastException = std::bad_any_cast;

    using std::make_any;
    using std::any_cast;

    using std::swap;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
