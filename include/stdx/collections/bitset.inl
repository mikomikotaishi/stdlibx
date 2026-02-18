#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using ::core::collections::BitSet;

    using ::core::collections::operator&;
    using ::core::collections::operator|;
    using ::core::collections::operator^;
    using ::core::collections::operator<<;
    using ::core::collections::operator>>;

    using ::core::collections::hash;
    using ::core::collections::swap;
}
