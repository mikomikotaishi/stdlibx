#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ErrorCategory = std::error_category;
    using ErrorCondition = std::error_condition;
    using ErrorCode = std::error_code;

    using SystemException = std::system_error;

    using std::generic_category;
    using std::system_category;
    using std::make_error_code;
    using std::make_error_condition;
}

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    template <typename T>
    using IsErrorCodeEnum = std::is_error_code_enum<T>;

    template <typename T>
    constexpr bool IsErrorCodeEnumValue = std::is_error_code_enum_v<T>;

    template <typename T>
    using IsErrorConditionEnum = std::is_error_condition_enum<T>;

    template <typename T>
    constexpr bool IsErrorConditionEnumValue = std::is_error_condition_enum_v<T>;
}
