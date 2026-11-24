#pragma once

#ifdef STDLIBX_EXECUTION_AVAILABLE

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 *
 * The reactor's coroutine task type, split out from reactor.inl so it can be
 * named before the Reactor itself is defined: stdx::sys::Process::wait_async
 * returns one, and process.inl is included well ahead of the net headers.
 *
 * The forward declarations are exported because both classes are exported
 * where they are defined; a later exported redeclaration of an entity first
 * declared unexported is ill-formed.
 */
export namespace stdx::net {
    class Reactor;
    class ReactorScheduler;
}

/**
 * @namespace stdx::net
 * @brief Internal spellings of Reactor::Env and Reactor::Task.
 *
 * Not exported: users name these through @ref Reactor::Env and
 * @ref Reactor::Task, which alias them. They exist at namespace scope only so
 * that headers compiled before Reactor is complete can name the same types.
 */
namespace stdx::net {
    /**
     * @internal
     * @struct ReactorTaskEnv
     * @brief The task environment that pins a reactor coroutine's start
     * scheduler to the concrete @ref ReactorScheduler. Exposed as Reactor::Env.
     *
     * This is the fix for the crash that a plain Task (task<T, env<>>) hit:
     * with the default environment, stdexec picks task_scheduler as the start
     * scheduler and type-erases ReactorScheduler into it. Awaiting a
     * value-producing operation then copies that type-erased backend through a
     * __value_proxy_root whose teardown dereferenced a null dispatch pointer -
     * a SEGV in ~__value_proxy_root<__itask_scheduler_backend>, reached via the
     * task promise's await_transform.
     *
     * Naming ReactorScheduler as the start_scheduler_type keeps the task's
     * scheduler concrete: no task_scheduler, no type-erased proxy, so the
     * crashing teardown path does not exist. ReactorScheduler is trivially
     * copyable (it holds one Reactor*), so the hops stdexec inserts to return
     * to it carry no lifetime hazard.
     */
    struct ReactorTaskEnv {
        using start_scheduler_type = ReactorScheduler;
    };

    /**
     * @internal
     * @brief A coroutine task pinned to a ReactorScheduler start scheduler.
     * Exposed as Reactor::Task.
     */
    template <typename T>
    using ReactorTask = stdx::exec::Task<T, ReactorTaskEnv>;
}

#endif
