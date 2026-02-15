/**
 * @file sequence.cppm
 * @module stdx:core.sequence
 * @brief Module file for standard library sequence operations.
 *
 * This file contains the implementation of the sequence operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.sequence;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::IntegerSequence;
    using ::core::make_integer_sequence;

    using ::core::IndexSequence;
    using ::core::IndexSequenceFor;
    using ::core::make_index_sequence;

    using ::core::begin;
    using ::core::end;
    using ::core::swap;
}
