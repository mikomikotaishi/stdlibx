#pragma once

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
