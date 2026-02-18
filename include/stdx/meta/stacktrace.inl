#pragma once

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    #if __has_include(<stacktrace>)
    using ::core::meta::StackTraceEntry;
    using ::core::meta::BasicStackTrace;
    using ::core::meta::StackTrace;
    
    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using ::core::meta::pmr::StackTrace;
    }

    using ::core::meta::hash;
    using ::core::meta::formatter;
    using ::core::meta::swap;

    using ::core::meta::to_string;
    using ::core::meta::operator<<;
    #endif
}
