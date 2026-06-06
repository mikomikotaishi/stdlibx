#pragma once

/**
 * @namespace stdx::debug
 * @brief Standard library debugging operations.
 */
export namespace stdx::debug {
    #if __has_include(<debugging>)
    using std::breakpoint;
    using std::breakpoint_if_debugging;
    using std::is_debugger_present;
    #endif
}
