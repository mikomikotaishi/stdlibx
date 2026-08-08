#pragma once

/**
 * @namespace stdx::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    // template <typename R, typename... Args>
    // using CoroutineTraits = std::coroutine_traits<R(Args...)>;

    template <typename Sig>
    using CoroutineTraits = std::coroutine_traits<Sig>;

    // template <typename R, typename... Args>
    // using CoroutineHandle = std::coroutine_handle<R(Args...)>;

    template <typename Sig>
    using CoroutineHandle = std::coroutine_handle<Sig>;

    using NoopCoroutinePromise = std::noop_coroutine_promise;
    using NoopCoroutineHandle = std::noop_coroutine_handle;
    using SuspendNever = std::suspend_never;
    using SuspendAlways = std::suspend_always;
}
