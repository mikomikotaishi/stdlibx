#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Expected;
    using ::core::Unexpected;
    using ::core::UnexpectTag;
    using ::core::Unexpect;
    using ::core::BadExpectedAccessException;
    using ::core::ErrorDescription;

    using ::core::swap;

    using ::core::Ok;
    using ::core::Err;
}
