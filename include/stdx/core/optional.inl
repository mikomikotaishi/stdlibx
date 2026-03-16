#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::Optional;
    using ::core::prelude::nullopt;
    using ::core::prelude::NullOption;
    using ::core::prelude::BadOptionalAccessException;

    using ::core::prelude::hash;
    using ::core::prelude::swap;

    using ::core::prelude::make_optional;
}
