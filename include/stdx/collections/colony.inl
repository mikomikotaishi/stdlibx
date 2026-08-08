#pragma once

using stdx::alloc::Allocator;
using stdx::meta::RemoveConstVolatile;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_hive
    using ColonyLimits = std::hive_limits;
    
    template <typename T, typename Alloc = Allocator<T>>
        requires 
            SameAs<typename RemoveConstVolatile<T>::type, T> &&
            SameAs<typename Alloc::value_type, T>
    using Colony = std::hive<T, Alloc>;
    #endif
}
