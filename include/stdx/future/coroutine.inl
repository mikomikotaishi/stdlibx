#pragma once

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
