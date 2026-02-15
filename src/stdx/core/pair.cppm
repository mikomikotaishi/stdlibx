/**
 * @file pair.cppm
 * @module stdx:core.pair
 * @brief Module file for standard library pair operations.
 *
 * This file contains the implementation of the pair operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.pair;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Pair;

    using ::core::swap;
    using ::core::get;

    using ::core::operator==;
    using ::core::operator<=>;
}
