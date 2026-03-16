#pragma once

/**
 * @namespace alloc::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace alloc::thread {
    #ifdef __cpp_lib_hazard_pointer
    template <typename T, typename Del = DefaultDelete<T>>
    using HazardPointerObjectBase = std::hazard_pointer_obj_base<T, Del>;

    using HazardPointer = std::hazard_pointer;

    using std::make_hazard_pointer;
    using std::swap;
    #endif
}
