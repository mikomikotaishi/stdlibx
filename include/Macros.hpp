/**
 * @file Macros.hpp
 * @brief Macros used in the library
 *
 * This file contains all macros that are used in this library.
 */

#pragma once

#if defined(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY) \
    || (defined(__cpp_senders) && defined(__cpp_lib_task) && defined(__cpp_lib_parallel_scheduler))
#define STDLIBX_EXECUTION_AVAILABLE
#endif

#ifdef __cpp_lib_reflection
#define THROWS(...) [[=Throws<__VA_ARGS__>]]
#else
#define THROWS(...)
#endif

/**
 * @brief A disabled THROWS annotation: always expands to nothing.
 *
 * Rename a THROWS to THROWS_DISABLED to suppress emission of the annotation
 * at that site while keeping the exception types visible for documentation.
 */
#define THROWS_DISABLED(...)

/**
 * @brief A deleted-function definition that carries the reason it is deleted.
 * @param ... The reason, as a string literal.
 *
 * Guarded on __cpp_deleted_function because it is a compiler feature-test macro
 * rather than a library one, and so is visible inside a module translation unit
 * where <version> has not been included. GCC 16 and clang 22 both report 202403.
 */
#ifdef __cpp_deleted_function
#define DELETE_METHOD(...) delete(__VA_ARGS__)
#else
#define DELETE_METHOD(...) delete
#endif
