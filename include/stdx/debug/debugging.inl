#pragma once

/**
 * @namespace stdx::debug
 * @brief Wrapper namespace for standard library debugging operations.
 */
export namespace stdx::debug {
    #ifdef __cpp_lib_debugging
    using ::core::debug::breakpoint;
    using ::core::debug::breakpoint_if_debugging;
    using ::core::debug::is_debugger_present;
    #endif
}
