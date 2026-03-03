#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #if __has_include(<inplace_vector>)
    using ::alloc::collections::InPlaceVector;

    using ::alloc::collections::erase;
    using ::alloc::collections::erase_if;
    #endif
}
