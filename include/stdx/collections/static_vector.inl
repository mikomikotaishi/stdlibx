#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_inplace_vector
    using ::alloc::collections::StaticVector;

    using ::alloc::collections::erase;
    using ::alloc::collections::erase_if;
    #endif
}
