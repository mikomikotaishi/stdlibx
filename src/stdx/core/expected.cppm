/**
 * @file expected.cppm
 * @module stdx:core.expected
 * @brief Module file for importing the Expected class from the standard library.
 * 
 * This file exports the Expected class operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.expected;

import core;

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
