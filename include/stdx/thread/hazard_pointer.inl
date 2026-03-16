#pragma once

/**
 * @namespace stdx::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace stdx::thread {
    #ifdef __cpp_lib_hazard_pointer
    using ::alloc::thread::HazardPointerObjectBase;
    using ::alloc::thread::HazardPointer;

    using ::alloc::thread::make_hazard_pointer;
    using ::alloc::thread::swap;
    #endif
}
