#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using ::alloc::mem::get_new_handler;
    using ::alloc::mem::set_new_handler;
    
    using ::alloc::mem::launder;
}
