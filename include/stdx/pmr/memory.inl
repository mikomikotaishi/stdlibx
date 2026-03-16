#pragma once

/**
 * @namespace stdx::pmr::mem
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::mem {
    #ifdef __cpp_lib_indirect
    using ::alloc::pmr::mem::Indirect;
    #endif

    #ifdef __cpp_lib_polymorphic
    using ::alloc::pmr::mem::Polymorphic;
    #endif
}
