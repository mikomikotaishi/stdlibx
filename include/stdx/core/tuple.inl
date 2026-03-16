#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::Tuple;
    using ::core::prelude::TupleSize;
    using ::core::prelude::TupleSizeValue;
    using ::core::prelude::TupleElement;
    using ::core::prelude::TupleElementType;

    using ::core::prelude::operator==;
    using ::core::prelude::operator<=>;

    using ::core::prelude::IgnoreType;
    using ::core::prelude::IGNORE;

    using ::core::prelude::make_tuple;
    using ::core::prelude::tie;
    using ::core::prelude::forward_as_tuple;
    using ::core::prelude::tuple_cat;
    using ::core::prelude::get;
    using ::core::prelude::apply;
    using ::core::prelude::make_from_tuple;

    using ::core::prelude::swap;
}
