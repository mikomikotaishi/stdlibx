#pragma once

/**
 * @namespace core::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace core::util {
    #if __has_include(<debugging>)
    using std::breakpoint;
    using std::breakpoint_if_debugging;
    using std::is_debugger_present;
    #endif
}
