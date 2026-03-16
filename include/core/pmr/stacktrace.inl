#pragma once

/**
 * @namespace core::pmr::meta
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace core::pmr::meta {
    #ifdef __cpp_lib_stacktrace
    using StackTrace = std::pmr::stacktrace;
    #endif
}