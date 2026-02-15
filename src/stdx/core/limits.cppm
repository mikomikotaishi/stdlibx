/**
 * @file limits.cppm
 * @module stdx:core.limits
 * @brief Module file for standard library limit operations.
 *
 * This file contains the implementation of the limit operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.limits;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::NumericLimits;
    using ::core::FloatRoundStyle;
    using ::core::FloatDenormStyle;
}
