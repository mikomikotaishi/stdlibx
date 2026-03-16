#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::Array;

    using ::core::prelude::operator==;
    using ::core::prelude::operator<=>;
    using ::core::prelude::to_array;
    using ::core::prelude::get;
    
    using ::core::prelude::begin;
    using ::core::prelude::cbegin;
    using ::core::prelude::end;
    using ::core::prelude::cend;
    using ::core::prelude::rbegin;
    using ::core::prelude::crbegin;
    using ::core::prelude::rend;
    using ::core::prelude::crend;
    using ::core::prelude::size;
    using ::core::prelude::ssize;
    using ::core::prelude::empty;
    using ::core::prelude::data;

    using ::core::prelude::swap;
}
