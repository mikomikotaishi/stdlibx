/**
 * @file tuple.cppm
 * @module stdx:core.tuple
 * @brief Module file for standard library tuple operations.
 *
 * This file contains the implementation of the tuple operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.tuple;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Tuple;
    using ::core::TupleSize;
    using ::core::TupleSizeValue;
    using ::core::TupleElement;
    using ::core::TupleElementType;

    using ::core::operator==;
    using ::core::operator<=>;

    using ::core::make_tuple;
    using ::core::tie;
    using ::core::forward_as_tuple;
    using ::core::tuple_cat;
    using ::core::get;
    using ::core::apply;
    using ::core::make_from_tuple;

    using ::core::swap;
}
