#pragma once

/**
 * @namespace stdx::debug
 * @brief Wrapper namespace for standard library debugging operations.
 */
export namespace stdx::debug {
    #ifdef __cpp_lib_stacktrace
    using ::core::debug::StackTraceEntry;
    using ::core::debug::BasicStackTrace;
    using ::core::debug::StackTrace;

    using ::core::debug::hash;
    using ::core::debug::formatter;
    using ::core::debug::swap;

    using ::core::debug::to_string;
    using ::core::debug::operator<<;
    #endif
}
