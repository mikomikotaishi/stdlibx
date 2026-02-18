#pragma once

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    #if __has_include(<stacktrace>)
    using StackTraceEntry = std::stacktrace_entry;

    template <typename Allocator>
    using BasicStackTrace = std::basic_stacktrace<Allocator>;

    using StackTrace = std::stacktrace;
    
    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using StackTrace = std::pmr::stacktrace;
    }

    using std::hash;
    using std::formatter;
    using std::swap;

    using std::to_string;
    using std::operator<<;
    #endif
}
