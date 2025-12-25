/**
 * @file system_error.cppm
 * @module std:core.system_error
 * @brief Module file for standard library system error classes.
 *
 * This file contains the implementation of error classes in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.system_error;
#else
export module stdlib:core.system_error;
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
    using ::core::ErrorCategory;
    using ::core::ErrorCondition;
    using ::core::ErrorCode;
    using ::core::Errc;
    using ::core::SystemException;
    using ::core::IsErrorCodeEnum;
    using ::core::IsErrorCodeEnumValue;
    using ::core::IsErrorConditionEnum;
    using ::core::IsErrorConditionEnumValue;

    using ::core::generic_category;
    using ::core::system_category;
    using ::core::make_error_code;
    using ::core::make_error_condition;

    using ::core::hash;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
