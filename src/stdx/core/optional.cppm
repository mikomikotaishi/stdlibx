/**
 * @file optional.cppm
 * @module stdx:core.optional
 * @brief Module file for importing the Optional class from the standard library.
 * 
 * This file exports the Optional class operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.optional;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Optional;
    using ::core::nullopt;
    using ::core::NullOption;
    using ::core::BadOptionalAccessException;

    using ::core::hash;
    using ::core::swap;

    using ::core::make_optional;
}
