#pragma once

using stdx::alloc::Allocator;
using stdx::meta::RemoveConstVolatile;

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    template <typename T, typename Alloc = Allocator<T>>
        requires 
            SameAs<typename RemoveConstVolatile<T>::type, T> &&
            SameAs<typename Alloc::value_type, T>
    using SinglyLinkedList = std::forward_list<T, Alloc>;
}
