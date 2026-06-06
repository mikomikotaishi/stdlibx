#pragma once

/**
 * @namespace stdx::debug
 * @brief Standard library debugging operations.
 */
export namespace stdx::debug {
    #ifdef __cpp_lib_stacktrace
    using StackTraceEntry = std::stacktrace_entry;

    template <typename Allocator>
    using BasicStackTrace = std::basic_stacktrace<Allocator>;

    using StackTrace = std::stacktrace;
    #endif
}
