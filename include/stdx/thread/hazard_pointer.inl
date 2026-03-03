#pragma once

using stdx::alloc::DefaultDelete;

/**
 * @namespace stdx::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace stdx::thread {
    #if __has_include(<hazard_pointer>)
    template <typename T, typename Deleter = DefaultDelete<T>>
    using HazardPointerObjectBase = std::hazard_pointer_obj_base<T, Deleter>;

    using HazardPointer = std::hazard_pointer;

    using std::make_hazard_pointer;
    using std::swap;
    #endif
}
