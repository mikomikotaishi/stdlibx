#pragma once

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace stdx::util {
    #ifdef __cpp_lib_debugging
    using ::core::util::breakpoint;
    using ::core::util::breakpoint_if_debugging;
    using ::core::util::is_debugger_present;
    #endif
}
