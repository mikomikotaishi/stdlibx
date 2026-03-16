#pragma once

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    #ifdef __cpp_lib_stacktrace
    using ::core::meta::StackTraceEntry;
    using ::core::meta::BasicStackTrace;
    using ::core::meta::StackTrace;

    using ::core::meta::hash;
    using ::core::meta::formatter;
    using ::core::meta::swap;

    using ::core::meta::to_string;
    using ::core::meta::operator<<;
    #endif
}
