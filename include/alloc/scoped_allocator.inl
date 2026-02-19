#pragma once

/**
 * @namespace alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace alloc {
    template <typename OuterAlloc, typename... InnerAllocs>
    using ScopedAllocatorAdaptor = std::scoped_allocator_adaptor<OuterAlloc, InnerAllocs...>;
}
