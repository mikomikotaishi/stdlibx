#pragma once

using core::meta::IsSameValue;
using core::meta::RemoveConstVolatile;

using alloc::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    #ifdef __cpp_lib_hive
    using ColonyLimits = std::hive_limits;
    
    template <typename T, typename Alloc = Allocator<T>>
        requires 
            IsSameValue<typename RemoveConstVolatile<T>::type, T> &&
            IsSameValue<typename Alloc::value_type, T>
    using Colony = std::hive<T, Alloc>;

    using std::swap;
    using std::erase;
    using std::erase_if;
    #endif
}
