#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::Variant;
    using ::core::prelude::Monostate;
    using ::core::prelude::VariantSize;
    using ::core::prelude::VariantAlternative;
    using ::core::prelude::VariantAlternativeType;
    using ::core::prelude::BadVariantAccessException;

    using ::core::prelude::hash;

    using ::core::prelude::VariantNoPosition;

    using ::core::prelude::visit;
    using ::core::prelude::holds_alternative;
    using ::core::prelude::get;
    using ::core::prelude::get_if;
    using ::core::prelude::swap;
}
