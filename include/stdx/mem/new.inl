#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using std::get_new_handler;
    using std::set_new_handler;
    
    using std::launder;
}
