/**
 * @file optional.cppm
 * @module core:optional
 * @brief Module file for importing the Optional class from the standard library.
 * 
 * This file exports the Optional class operations in the standard library.
 */

module;

#include <optional>

#include "Macros.hpp"

export module core:optional;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    using Optional = std::optional<T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using NullOpt = std::nullopt_t;
    using NullOpt_t = std::nullopt_t;
    using NullOption_t = std::nullopt_t;
    #endif

    using std::nullopt;
    using NullOption = std::nullopt_t;

    using BadOptionalAccessException = std::bad_optional_access;

    using std::hash;
    using std::swap;

    using std::make_optional;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
