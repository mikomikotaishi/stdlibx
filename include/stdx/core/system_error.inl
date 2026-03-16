#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::ErrorCategory;
    using ::core::prelude::ErrorCondition;
    using ::core::prelude::ErrorCode;
    using ::core::prelude::Errc;
    using ::core::prelude::SystemException;

    using ::core::prelude::generic_category;
    using ::core::prelude::system_category;
    using ::core::prelude::make_error_code;
    using ::core::prelude::make_error_condition;

    using ::core::prelude::hash;
}

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    using ::core::meta::IsErrorCodeEnum;
    using ::core::meta::IsErrorCodeEnumValue;
    using ::core::meta::IsErrorConditionEnum;
    using ::core::meta::IsErrorConditionEnumValue;
}
