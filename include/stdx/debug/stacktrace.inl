#pragma once

/**
 * @namespace stdx::debug
 * @brief Wrapper namespace for standard library debugging operations.
 */
export namespace stdx::debug {
    #ifdef __cpp_lib_stacktrace
    using StackTraceEntry = std::stacktrace_entry;

    template <typename Allocator>
    using BasicStackTrace = std::basic_stacktrace<Allocator>;

    using StackTrace = std::stacktrace;

    using std::hash;
    using std::formatter;
    using std::swap;

    using std::to_string;
    using std::operator<<;
    #endif
}
