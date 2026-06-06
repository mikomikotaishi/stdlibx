#pragma once

/**
 * @namespace stdx::alloc
 * @brief Standard library allocation operations.
 */
export namespace stdx::alloc {
    template <typename OuterAlloc, typename... InnerAllocs>
    using ScopedAllocatorAdaptor = std::scoped_allocator_adaptor<OuterAlloc, InnerAllocs...>;
}
