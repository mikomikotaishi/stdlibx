#pragma once

/**
 * @namespace alloc::pmr::mem
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::pmr::mem {
    #ifdef __cpp_lib_indirect
    template <typename T>
    using Indirect = std::pmr::indirect<T>;
    #endif

    #ifdef __cpp_lib_polymorphic
    template <typename T>
    using Polymorphic = std::pmr::polymorphic<T>;
    #endif
}
