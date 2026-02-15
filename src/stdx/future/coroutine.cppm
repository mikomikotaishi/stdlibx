/**
 * @file coroutine.cppm
 * @module stdx:future.coroutine
 * @brief Module file for standard library coroutine operations.
 *
 * This file contains the implementation of the coroutine operations in the standard library.
 */

module;

#if __has_include(<coroutine>)
#include <coroutine>
#endif

export module stdx:future.coroutine;

/**
 * @namespace stdx::future
 * @brief Wrapper namespace for standard library concurrency operations.
 */
export namespace stdx::future {
    #if __has_include(<coroutine>)
    // template <typename R, typename... Args>
    // using CoroutineTraits = std::coroutine_traits<R(Args...)>;

    template <typename Signature>
    using CoroutineTraits = std::coroutine_traits<Signature>;

    // template <typename R, typename... Args>
    // using CoroutineHandle = std::coroutine_handle<R(Args...)>;

    template <typename Signature>
    using CoroutineHandle = std::coroutine_handle<Signature>;

    using NoopCoroutinePromise = std::noop_coroutine_promise;
    using NoopCoroutineHandle = std::noop_coroutine_handle;
    using SuspendNever = std::suspend_never;
    using SuspendAlways = std::suspend_always;

    using std::noop_coroutine;

    using std::hash;
    #endif
}
