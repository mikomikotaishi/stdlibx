#pragma once

using stdx::alloc::Allocator;
using stdx::meta::IsSameValue;
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
            IsSameValue<typename RemoveConstVolatile<T>::type, T> &&
            IsSameValue<typename Alloc::value_type, T>
    using Colony = std::hive<T, Alloc>;
    #endif
}
