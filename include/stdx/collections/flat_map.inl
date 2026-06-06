#pragma once

using stdx::meta::IsNothrowSwappableValue;
using stdx::meta::IsSameValue;
using stdx::meta::RemoveConstVolatile;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_flat_map
    template <typename Key, typename Value, typename Compare = Less<Key>, typename KeyContainer = Vector<Key>, typename ValueContainer = Vector<Value>>
        requires
            IsSameValue<Key, typename KeyContainer::value_type> && 
            IsSameValue<Value, typename ValueContainer::value_type> &&
            IsNothrowSwappableValue<KeyContainer> &&
            IsNothrowSwappableValue<ValueContainer>
    using FlatMap = std::flat_map<Key, Value, Compare, KeyContainer, ValueContainer>;

    template <typename Key, typename Value, typename Compare = Less<Key>, typename KeyContainer = Vector<Key>, typename ValueContainer = Vector<Value>>
        requires
            IsSameValue<Key, typename KeyContainer::value_type> && 
            IsSameValue<Value, typename ValueContainer::value_type> &&
            IsNothrowSwappableValue<KeyContainer> &&
            IsNothrowSwappableValue<ValueContainer>
    using FlatMultimap = std::flat_multimap<Key, Value, Compare, KeyContainer, ValueContainer>;
    #endif
}
