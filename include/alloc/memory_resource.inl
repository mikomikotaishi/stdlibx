#pragma once

/**
 * @namespace alloc::mem
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::pmr {
    template <typename T>
    using PolymorphicAllocator = std::pmr::polymorphic_allocator<T>;
}
