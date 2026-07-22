#pragma once

using stdx::alloc::BadAllocationException;
using stdx::time::Duration;
using stdx::time::Instant;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::Sender;
using stdx::exec::SyncWait;
using stdx::exec::SyncWaitWithVariant;
#endif

/**
 * @namespace stdx::thread
 * @brief Standard library threading operations (internal traits).
 */
namespace stdx::thread {
    /**
     * @internal
     * @concept StandardThread
     * @brief The set of standard thread types BasicThread is intended to wrap.
     * @tparam Thr The candidate underlying thread type.
     */
    template <typename Thr>
    concept StandardThread = SameAs<Thr, std::jthread> || SameAs<Thr, std::thread>;

    /**
     * @internal
     * @concept Stoppable
     * @brief Whether the wrapped thread type carries cooperative-cancellation
     * state (std::jthread's shared stop-source), so the stop API can be exposed.
     * @tparam Thr The underlying thread type.
     */
    template <typename Thr>
    concept Stoppable = requires (Thr& t, const Thr& ct) {
        t.request_stop();
        t.get_stop_source();
        ct.get_stop_token();
    };

    /**
     * @internal
     * @class BasicThread
     * @brief A uniform, move-only wrapper over a standard thread type.
     * @tparam Thr The underlying thread type (std::thread or std::jthread).
     */
    template <StandardThread Thr>
    class [[nodiscard]] BasicThread {
    public:
        using Self = Thr; ///< The underlying thread type being wrapped.
        using Id = Thr::id; ///< The thread identifier type.
        using NativeHandle = Thr::native_handle_type; ///< The implementation-defined handle type.
    private:
        Thr _thread; ///< The wrapped thread.
    public:
        /**
         * @brief Constructs a wrapper with no associated thread of execution.
         */
        BasicThread() noexcept = default;

        /**
         * @brief Starts a new thread running func(args...).
         * @tparam Fn The callable type.
         * @tparam Args The argument types forwarded to the callable.
         * @param func The callable to run on the new thread.
         * @param args The arguments to invoke @p func with.
         *
         * For a std::jthread-backed thread, @p func may optionally take a
         * StopToken as its first parameter to observe cooperative cancellation.
         */
        template <typename Fn, typename... Args>
            requires ConstructibleFrom<Thr, Fn, Args...>
        explicit BasicThread(Fn&& func, Args&&... args):
            _thread{Ops::forward<Fn>(func), Ops::forward<Args>(args)...} {}

        /**
         * @brief Whether this object identifies an active thread of execution.
         * @return true if the thread is joinable.
         */
        [[nodiscard]]
        bool joinable() const noexcept {
            return _thread.joinable();
        }

        /**
         * @brief Blocks until the thread finishes execution.
         */
        THROWS(SystemException)
        void join() {
            _thread.join();
        }

        /**
         * @brief Detaches the thread, letting it continue independently.
         */
        THROWS(SystemException)
        void detach() {
            _thread.detach();
        }

        /**
         * @brief Converts the wrapper to the underlying thread type.
         * @return The underlying thread.
         */
        [[nodiscard]]
        constexpr operator Thr() const noexcept {
            return _thread;
        }

        /**
         * @brief The identifier of the thread.
         * @return The thread's Id, or a default-constructed Id if not joinable.
         */
        [[nodiscard]]
        Id id() const noexcept {
            return _thread.get_id();
        }

        /**
         * @brief The implementation-defined native thread handle.
         * @return The native handle.
         */
        [[nodiscard]]
        NativeHandle native_handle() {
            return _thread.native_handle();
        }

        /**
         * @brief Swaps the underlying threads of two wrappers.
         * @param other The wrapper to swap with.
         */
        void swap(BasicThread& other) noexcept {
            _thread.swap(other._thread);
        }

        /**
         * @brief Swaps two wrappers.
         * @param lhs The first wrapper.
         * @param rhs The second wrapper.
         */
        friend void swap(BasicThread& lhs, BasicThread& rhs) noexcept {
            lhs.swap(rhs);
        }

        /**
         * @brief Requests cooperative cancellation of the thread. Stop-aware
         * threads only.
         * @return true if this call made the stop request (none was pending).
         */
        bool request_stop() noexcept requires Stoppable<Thr> {
            return _thread.request_stop();
        }

        /**
         * @brief The stop_source sharing the thread's stop-state. Stop-aware
         * threads only.
         * @return The associated StopSource.
         */
        [[nodiscard]]
        StopSource stop_source() noexcept requires Stoppable<Thr> {
            return _thread.get_stop_source();
        }

        /**
         * @brief A stop_token observing the thread's stop-state. Stop-aware
         * threads only.
         * @return The associated StopToken.
         */
        [[nodiscard]]
        StopToken stop_token() const noexcept
            requires Stoppable<Thr> {
            return _thread.get_stop_token();
        }

        /**
         * @brief A hint of the number of concurrent hardware thread contexts.
         * @return The hardware concurrency, or 0 if not computable.
         */
        [[nodiscard]]
        static u32 hardware_concurrency() noexcept {
            return Thr::hardware_concurrency();
        }

        /**
         * @brief The ID of the current thread.
         * @return ThreadId The thread ID.
         */
        [[nodiscard]]
        static Id current_id() noexcept {
            return std::this_thread::get_id();
        }

        /**
         * @brief Sleep for the given duration.
         * @tparam Rep The duration representation type.
         * @tparam Period The duration period type.
         * @param duration The duration to sleep for.
         */
        template <typename Rep, typename Period>
        static void sleep_for(const Duration<Rep, Period>& duration) {
            std::this_thread::sleep_for(duration);
        }

        /**
         * @brief Sleep until the given time point. The clock of the time point determines the clock used for sleeping.
         * @tparam Clock The clock type of the time point.
         * @tparam Dur The duration type of the time point.
         * @param time_point The time point to sleep until.
         */
        template <typename Clock, typename Dur>
        static void sleep_until(const Instant<Clock, Dur>& time_point) {
            std::this_thread::sleep_until(time_point);
        }

        /**
         * @brief Yield execution to another thread. Does not guarantee any particular scheduling behavior.
         */
        static void yield() noexcept {
            std::this_thread::yield();
        }

        /**
         * @brief Defers or spawns a callable on a new thread by calling std::async.
         * @tparam F A callable type.
         * @tparam Args The argument types for the callable.
         * @param f The callable to invoke.
         * @param args The arguments to pass to the callable.
         * @return A Future representing the result of the callable.
         * @throws SystemException if the thread cannot be created.
         * @throws BadAllocationException if memory allocation fails.
         */
        template <typename F, typename... Args>
        THROWS(SystemException, BadAllocationException)
        static Future<InvokeResultType<DecayType<F>, DecayType<Args>...>> defer_or_spawn(F&& f, Args&&... args) {
            return std::async(forward<F>(f), forward<Args>(args)...);
        }

        /**
         * @brief Defers or spawns a callable on a new thread by calling std::async with a launch policy.
         * @tparam F A callable type.
         * @tparam Args The argument types for the callable.
         * @param policy The launch policy (e.g., std::launch::async or std::launch::deferred).
         * @param f The callable to invoke.
         * @param args The arguments to pass to the callable.
         * @return A Future representing the result of the callable.
         * @throws SystemException if the thread cannot be created.
         * @throws BadAllocationException if memory allocation fails.
         *
         * @note The launch policy determines whether the callable is executed asynchronously on a new thread or deferred until the result is needed.
         */
        template <typename F, typename... Args>
        THROWS(SystemException, BadAllocationException)
        static Future<InvokeResultType<DecayType<F>, DecayType<Args>...>> defer_or_spawn(LaunchPolicy policy, F&& f, Args&&... args) {
            return std::async(policy, forward<F>(f), forward<Args>(args)...);
        }

        #ifdef STDLIBX_EXECUTION_AVAILABLE
        /**
         * @brief Synchronously wait for the result of a sender.
         * @tparam Sender The type of the sender.
         * @param sender The sender to wait for.
         * @return The result of the sender (Optional<Tuple<Ts...>>), or nullopt if the operation failed.
         */
        static auto sync_wait(Sender auto sender) {
            return SyncWait(Ops::move(sender));
        }

        /**
         * @brief Synchronously wait for the result of a sender, returning a variant.
         * @tparam Sender The type of the sender.
         * @param sender The sender to wait for.
         * @return The result of the sender (Optional<Variant<Ts...>>), or nullopt if the operation failed.
         */
        static auto sync_wait_with_variant(Sender auto sender) {
            return SyncWaitWithVariant(Ops::move(sender));
        }

        [[nodiscard]]
        static ParallelScheduler parallel_scheduler() {
            return stdx::exec::get_parallel_scheduler();
        }

        /**
         * @brief Offloads a blocking callable onto the system thread pool.
         * @tparam F A nullary callable.
         * @param f The blocking work to run off the calling thread.
         * @return A sender completing with the result of f() (or set_error if it
         * throws; the exception is re-raised at the await / sync_wait site).
         *
         * @warning Do not compose two of these senders under WhenAll.
         * To overlap two offloads, use the Task-based `Ops::offload` and
         * `WhenAll(offload(f), offload(g))` instead. Likewise a `void` callable
         *  under `Thread::sync_wait()` trips the same move miscompile — use `offload` there.
         */
        template <Invocable F>
        [[nodiscard]]
        static Sender auto offload_sender(F f) {
            return Then(Schedule(parallel_scheduler()), Ops::move(f));
        }

        /**
         * @brief Coroutine bridge: run a blocking callable on the parallel scheduler.
         * @tparam F A nullary callable.
         * @param f The blocking work to run off the calling thread.
         * @return A Task completing with the result of f() (or set_error if it throws;
         * the exception is re-raised at the await / sync_wait site).
         *
         * @warning Do not `co_await offload(...)` inside another Task.
         * To co_await an offload from inside a Task, use the
         * sender-based `Ops::offload_sender`.
         */
        template <Invocable F>
        static Task<InvokeResultType<F&>> offload(F f) {
            co_await Schedule(parallel_scheduler());
            co_return f();
        }
        #endif
    };

    export using ManualThread = BasicThread<std::thread>;
    export using Thread = BasicThread<std::jthread>;
}
