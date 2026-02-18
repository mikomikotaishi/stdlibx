#pragma once

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    template <typename OuterAlloc, typename... InnerAllocs>
    using ScopedAllocatorAdaptor = std::scoped_allocator_adaptor<OuterAlloc, InnerAllocs...>;
}
