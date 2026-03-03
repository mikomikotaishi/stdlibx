#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_hive
    using ::alloc::collections::ColonyLimits;
    using ::alloc::collections::Colony;
    using ::alloc::collections::swap;
    using ::alloc::collections::erase;
    using ::alloc::collections::erase_if;
    #endif
}
