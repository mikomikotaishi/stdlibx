#pragma once

/**
 * @namespace core::debug
 * @brief Wrapper namespace for standard library debugging operations.
 */
export namespace core::debug {
    #if __has_include(<debugging>)
    using std::breakpoint;
    using std::breakpoint_if_debugging;
    using std::is_debugger_present;
    #endif
}
