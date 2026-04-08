#pragma once

namespace stdx::os::unix {
    constexpr auto TssDtorIterations = TSS_DTOR_ITERATIONS;
}

#undef TSS_DTOR_ITERATIONS

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    using C11Thread = ::thrd_t;
    using C11Mutex = ::mtx_t;
    using C11ConditionVariable = ::cnd_t;
    using C11ThreadFunction = ::thrd_start_t;
    using C11ThreadSpecificKey = ::tss_t;
    using C11DestructorFunction = ::tss_dtor_t;

    using ::thrd_create;
    using ::thrd_equal;
    using ::thrd_current;
    using ::thrd_sleep;
    using ::thrd_yield;
    using ::thrd_exit;
    using ::thrd_detach;
    using ::thrd_join;
    constexpr auto THREAD_SUCCESS = ::thrd_success;
    constexpr auto THREAD_TIMEDOUT = ::thrd_timedout;
    constexpr auto THREAD_ERROR = ::thrd_error;
    constexpr auto THREAD_NO_MEMORY = ::thrd_nomem;
    constexpr auto THREAD_BUSY = ::thrd_busy;

    using ::mtx_init;
    using ::mtx_lock;
    using ::mtx_timedlock;
    using ::mtx_trylock;
    using ::mtx_unlock;
    using ::mtx_destroy;
    constexpr auto MUTEX_PLAIN = ::mtx_plain;
    constexpr auto MUTEX_RECURSIVE = ::mtx_recursive;
    constexpr auto MUTEX_TIMED = ::mtx_timed;

    using ::call_once;

    using ::cnd_init;
    using ::cnd_signal;
    using ::cnd_broadcast;
    using ::cnd_wait;
    using ::cnd_timedwait;
    using ::cnd_destroy;

    using ::tss_create;
    using ::tss_delete;
    using ::tss_get;
    using ::tss_set;
    using ::tss_delete;

    constexpr auto TSS_DTOR_ITERATIONS = TssDtorIterations;
}
