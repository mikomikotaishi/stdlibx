#pragma once

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
