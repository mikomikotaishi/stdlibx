#pragma once

using stdx::time::Duration;
using stdx::time::Instant;

/**
 * @namespace stdx::thread
 * @brief Standard library threading operations (internal traits).
 */
namespace stdx::thread {
    /**
     * @concept StandardThread
     * @brief The set of standard thread types BasicThread is intended to wrap.
     * @tparam Thr The candidate underlying thread type.
     */
    template <typename Thr>
    concept StandardThread = SameAs<Thr, std::jthread> || SameAs<Thr, std::thread>;

    /**
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
}

/**
 * @namespace stdx::thread
 * @brief Standard library threading operations.
 */
export namespace stdx::thread {
    /**
     * @class BasicThread
     * @brief A uniform, move-only wrapper over a standard thread type.
     * @tparam Thr The underlying thread type (std::thread or std::jthread).
     */
    template <StandardThread Thr>
    class BasicThread {
    public:
        using Id = Thr::id; ///< The thread identifier type.
        using NativeHandle = Thr::native_handle_type; ///< The implementation-defined handle type.
        using Of = Thr; ///< The underlying thread type being wrapped.
    private:
        Thr thread; ///< The wrapped thread.
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
            thread{Ops::forward<Fn>(func), Ops::forward<Args>(args)...} {}

        /**
         * @brief Whether this object identifies an active thread of execution.
         * @return true if the thread is joinable.
         */
        [[nodiscard]]
        bool joinable() const noexcept {
            return thread.joinable();
        }

        /**
         * @brief Blocks until the thread finishes execution.
         */
        THROWS(SystemException)
        void join() {
            thread.join();
        }

        /**
         * @brief Detaches the thread, letting it continue independently.
         */
        THROWS(SystemException)
        void detach() {
            thread.detach();
        }

        /**
         * @brief The identifier of the thread.
         * @return The thread's Id, or a default-constructed Id if not joinable.
         */
        [[nodiscard]]
        Id id() const noexcept {
            return thread.get_id();
        }

        /**
         * @brief The implementation-defined native thread handle.
         * @return The native handle.
         */
        [[nodiscard]]
        NativeHandle native_handle() {
            return thread.native_handle();
        }

        /**
         * @brief Swaps the underlying threads of two wrappers.
         * @param other The wrapper to swap with.
         */
        void swap(BasicThread& other) noexcept {
            thread.swap(other.thread);
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
            return thread.request_stop();
        }

        /**
         * @brief The stop_source sharing the thread's stop-state. Stop-aware
         * threads only.
         * @return The associated StopSource.
         */
        [[nodiscard]]
        StopSource stop_source() noexcept requires Stoppable<Thr> {
            return thread.get_stop_source();
        }

        /**
         * @brief A stop_token observing the thread's stop-state. Stop-aware
         * threads only.
         * @return The associated StopToken.
         */
        [[nodiscard]]
        StopToken stop_token() const noexcept
            requires Stoppable<Thr> {
            return thread.get_stop_token();
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
    };

    using ManualThread = BasicThread<std::thread>;
    using Thread = BasicThread<std::jthread>;
}
