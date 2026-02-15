/**
 * @file stacktrace.cppm
 * @module stdx:meta.stacktrace
 * @brief Module file for standard library stacktrace operations.
 *
 * This file contains the implementation of the stack trace operations in the standard library.
 */

module;

export module stdx:meta.stacktrace;

import core;

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    #if __has_include(<stacktrace>)
    using core::meta::StackTraceEntry;
    using core::meta::BasicStackTrace;
    using core::meta::StackTrace;
    
    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using core::meta::pmr::StackTrace;
    }

    using core::meta::hash;
    using core::meta::formatter;
    using core::meta::swap;

    using core::meta::to_string;
    using core::meta::operator<<;
    #endif
}
