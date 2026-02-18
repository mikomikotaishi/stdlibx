#pragma once

using core::Less;
using core::meta::IsNothrowSwappableValue;
using core::meta::IsSameValue;

using alloc::mem::Allocator;

/**
 * @namespace alloc::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace alloc::collections {
    #if __has_include(<flat_map>)
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

    using std::erase_if;
    #endif
}
