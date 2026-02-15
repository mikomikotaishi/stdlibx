/**
 * @file system_error.cppm
 * @module stdx:core.system_error
 * @brief Module file for standard library system error classes.
 *
 * This file contains the implementation of error classes in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.system_error;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
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
