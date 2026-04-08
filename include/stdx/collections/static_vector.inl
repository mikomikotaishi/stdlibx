#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_inplace_vector
    using ::core::collections::StaticVector;

    using ::core::collections::erase;
    using ::core::collections::erase_if;
    #endif
}
