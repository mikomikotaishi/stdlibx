#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::Variant;
    using ::core::Monostate;
    using ::core::VariantSize;
    using ::core::VariantAlternative;
    using ::core::VariantAlternativeType;
    using ::core::BadVariantAccessException;

    using ::core::hash;

    using ::core::VariantNoPosition;

    using ::core::visit;
    using ::core::holds_alternative;
    using ::core::get;
    using ::core::get_if;
    using ::core::swap;
}
