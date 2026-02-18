#pragma once

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
