#pragma once

/**
 * @namespace stdx::pmr::meta
 * @brief Namespace for operations on polymorphic memory resources.
 */
namespace stdx::pmr::meta {
    #ifdef __cpp_lib_stacktrace
    using ::core::pmr::meta::StackTrace;
    #endif
}
