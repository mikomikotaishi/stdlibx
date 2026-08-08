#pragma once

#ifdef STDLIBX_EXECUTION_AVAILABLE

using stdx::exec::CompletionSignatures;
using stdx::exec::Connect;
using stdx::exec::OperationStateTag;
using stdx::exec::ReceiverTag;
using stdx::exec::SchedulerTag;
using stdx::exec::SenderTag;
using stdx::exec::SetError;
using stdx::exec::SetStopped;
using stdx::exec::SetValue;
using stdx::exec::Start;
using stdx::exec::Task;

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class ReactorScheduler
     * @brief A scheduler whose work runs on the thread driving a @ref Reactor.
     *
     * A coroutine Task needs a scheduler in its environment to resume on. This
     * is the one that keeps everything on the reactor's own thread, which is
     * what makes the whole design single-threaded: a Reactor is not thread-safe,
     * so a continuation that touched it from anywhere else would be a race.
     *
     * Work is posted as a zero-delay timer, so it runs on the reactor's next
     * turn rather than re-entering the loop from inside a handler.
     */
    class ReactorScheduler {
    private:
        Reactor* _reactor = nullptr; ///< The reactor to post work to; never null once constructed.

        /**
         * @internal
         * @brief The operation state for one scheduled continuation.
         * @tparam Rcvr The receiver to complete.
         */
        template <typename Rcvr>
        struct Operation {
            using operation_state_concept = OperationStateTag;

            Reactor* reactor; ///< Where the continuation is posted.
            Rcvr receiver; ///< Completed once the reactor reaches it.

            void start() & noexcept {
                try {
                    static_cast<void>(reactor->after(0ms, [this] -> void {
                        SetValue(Ops::move(receiver));
                    }));
                } catch (...) {
                    SetError(Ops::move(receiver), System::current_exception());
                }
            }
        };

        /**
         * @internal
         * @struct ScheduleSender
         * @brief What @ref ReactorScheduler::schedule returns.
         */
        struct ScheduleSender {
            using sender_concept = SenderTag;
            using completion_signatures = CompletionSignatures<
                stdx::exec::types::SetValue(),
                stdx::exec::types::SetError(ExceptionPointer)
            >;

            Reactor* reactor; ///< The reactor the work belongs to.

            template <typename Rcvr>
            [[nodiscard]]
            Operation<Rcvr> connect(Rcvr receiver) const {
                return Operation<Rcvr> {
                    .reactor = reactor,
                    .receiver = Ops::move(receiver)
                };
            }
        };
    public:
        using scheduler_concept = SchedulerTag;

        explicit ReactorScheduler(Reactor& reactor) noexcept:
            _reactor{&reactor} {}

        /**
         * @brief A sender that completes on the reactor's next turn.
         */
        [[nodiscard]]
        ScheduleSender schedule() const noexcept {
            return ScheduleSender {
                .reactor = _reactor,
            };
        }

        /**
         * @brief The reactor this scheduler posts to.
         */
        [[nodiscard]]
        Reactor& reactor() const noexcept {
            return *_reactor;
        }

        [[nodiscard]]
        friend bool operator==(const ReactorScheduler& left, const ReactorScheduler& right) noexcept {
            return left._reactor == right._reactor;
        }
    };
}

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class Readiness
     * @brief An awaitable that suspends until a descriptor is ready.
     *
     * The bridge between the Reactor's callbacks and a coroutine: registering
     * interest is what the handler does, and resuming the waiter is what it runs.
     * The registration is dropped again before the waiter resumes, so the same
     * descriptor can be awaited for the other direction immediately afterwards.
     *
     * Await it directly only to wait on readiness itself; the operations below
     * are what a caller normally wants.
     */
    class [[nodiscard]] Readiness {
    private:
        /**
         * @internal
         * @struct Operation
         * @brief One suspended wait: the registration, and who to resume.
         * @tparam Receiver The receiver to complete with the Event.
         */
        template <typename Rcvr>
        struct Operation {
            using operation_state_concept = OperationStateTag;

            Reactor* reactor; ///< The loop that will report the readiness.
            Socket::NativeHandle handle; ///< The descriptor being waited on; not owned.
            Interest interest; ///< What is being waited for.
            Rcvr receiver; ///< Completed with the Event that arrived.

            void start() & noexcept {
                try {
                    reactor->add(handle, interest, [this]([[maybe_unused]] const Event& event) -> void {
                        reactor->remove(handle);
                        SetValue(Ops::move(receiver));
                    });
                } catch (...) {
                    SetError(Ops::move(receiver), System::current_exception());
                }
            }
        };
    public:
        using sender_concept = SenderTag;
        using completion_signatures = CompletionSignatures<
            stdx::exec::types::SetValue(),
            stdx::exec::types::SetError(ExceptionPointer)
        >;

        Reactor* reactor; ///< The loop that will report the readiness.
        Socket::NativeHandle handle; ///< The descriptor to wait on.
        Interest interest; ///< What to wait for.

        Readiness(Reactor& loop, Socket::NativeHandle descriptor, Interest wanted) noexcept:
            reactor{&loop}, handle{descriptor}, interest{wanted} {}

        template <typename Rcvr>
        [[nodiscard]]
        Operation<Rcvr> connect(Rcvr receiver) const {
            return Operation<Rcvr> {
                .reactor = reactor,
                .handle = handle,
                .interest = interest,
                .receiver = Ops::move(receiver),
            };
        }
    };

    /**
     * @brief A sender that completes once @p handle has something to read.
     */
    inline Readiness Reactor::readable(NativeHandle handle) noexcept {
        return Readiness(*this, handle, Interest::READ);
    }

    /**
     * @brief A sender that completes once @p handle will accept a write.
     */
    inline Readiness Reactor::writable(NativeHandle handle) noexcept {
        return Readiness(*this, handle, Interest::WRITE);
    }

    /**
     * @brief Reads from @p stream, suspending until it has something to give.
     */
    template <ByteReader S>
    Task<usize> Reactor::receive(S& stream, Span<byte> into) {
        while (true) {
            if (const Optional<usize> read = stream.try_receive(into); read.has_value()) {
                co_return *read;
            }
            co_await Readiness(*this, stream.native_handle(), Interest::READ);
        }
    }

    /**
     * @brief Writes to @p stream, suspending until it will take bytes.
     */
    template <ByteWriter S>
    Task<usize> Reactor::send(S& stream, Span<const byte> from) {
        while (true) {
            if (const Optional<usize> written = stream.try_send(from); written.has_value()) {
                co_return *written;
            }
            co_await Readiness(*this, stream.native_handle(), Interest::WRITE);
        }
    }

    /**
     * @brief Writes all of @p from, suspending as often as it takes.
     */
    template <ByteWriter S>
    Task<usize> Reactor::send_all(S& stream, Span<const byte> from) {
        usize total = 0;
        while (total < from.size()) {
            const usize written = co_await send(stream, from.subspan(total));
            if (written == 0) {
                break;
            }
            total += written;
        }
        co_return total;
    }

    /**
     * @brief Accepts one connection, suspending until a peer arrives.
     */
    template <Acceptor L>
    Task<typename L::Stream> Reactor::accept(L& listener) {
        while (true) {
            if (Optional<typename L::Stream> accepted = listener.try_accept(); accepted.has_value()) {
                co_return Ops::move(*accepted);
            }
            co_await Readiness(*this, listener.native_handle(), Interest::READ);
        }
    }
}

namespace stdx::net {
    // ---------------------------------------------------------------------
    // Reactor::run_until is the one part still unfinished, and the cause is now
    // known. Task is not connected through connect() at all - it has no connect,
    // static or member - but through task::as_awaitable, which is constrained on
    // __has_compatible_environment_with<env_of_t<ParentPromise>, TaskEnv>
    // (stdexec/__detail/__task.hpp:349). Two things fail that:
    //
    //   1. The scheduler half needs task_scheduler(get_start_scheduler(env)).
    //      task_scheduler has no default constructor, so the "default
    //      initializable" escape does not apply, and ReactorEnv answers
    //      get_scheduler rather than get_start_scheduler. stdx::exec does not
    //      export the latter's query type yet either.
    //   2. That constructor also requires __infallible_scheduler, so a
    //      scheduler's schedule() sender may have neither an error nor a stopped
    //      completion - only set_value(). ScheduleSender above declares
    //      SetError(ExceptionPointer), which makes ReactorScheduler fallible and
    //      therefore unusable as a task's start scheduler. Note that completing
    //      with set_stopped instead is not a way out, and that the unstoppable
    //      environment the check runs under (__task::__env_t<true>, which pins
    //      the stop token to never_stop_token) does not help either: it only
    //      relaxes senders that compute their signatures from the environment,
    //      and ScheduleSender's are a fixed alias. Dropping the error completion
    //      means making a scheduler post that genuinely cannot fail, which is a
    //      design question about Reactor::after and not a rename.
    //
    // The failure surfaces as "no matching function for call to Connect" and
    // "no member named 'await_ready' in stdexec::task", neither of which points
    // anywhere near the environment, which is why this took a while to find.
    //
    // Nothing above this point depends on it. Until it is settled, drive the
    // operations with your own loop: start the coroutine, then pump
    // Reactor::run_once() until it completes.
    // ---------------------------------------------------------------------
    #ifdef STDLIBX_NET_ASYNC_TASKS
    /**
     * @internal
     * @struct ReactorEnv
     * @brief The receiver environment that hands a Task its scheduler.
     */
    struct ReactorEnv {
        ReactorScheduler scheduler; ///< Where continuations are resumed.

        [[nodiscard]]
        ReactorScheduler query(stdx::exec::types::GetScheduler) const noexcept {
            return scheduler;
        }
    };

    /**
     * @internal
     * @struct CollectReceiver
     * @brief Records a Task's completion so the driving loop can stop.
     * @tparam T The Task's value type.
     */
    template <typename T>
    struct CollectReceiver {
        using receiver_concept = ReceiverTag;

        Optional<T>* value; ///< Where a successful result is put.
        ExceptionPointer* error; ///< Set instead if the Task threw.
        bool* done; ///< Set on any completion, successful or not.
        ReactorScheduler scheduler; ///< Handed to the Task through @ref get_env.

        void set_value(T result) noexcept {
            value->emplace(Ops::move(result));
            *done = true;
        }

        void set_error(ExceptionPointer thrown) noexcept {
            *error = Ops::move(thrown);
            *done = true;
        }

        void set_stopped() noexcept {
            *done = true;
        }

        [[nodiscard]]
        ReactorEnv get_env() const noexcept {
            return ReactorEnv {
                .scheduler = scheduler,
            };
        }
    };
    #endif
}

export namespace stdx::net {
    #ifdef STDLIBX_NET_ASYNC_TASKS
    /**
     * @brief Runs @p task to completion, driving this Reactor until it finishes.
     */
    template <typename T>
    T Reactor::run_until(Task<T> task) {
        Optional<T> value;
        ExceptionPointer error;
        bool done = false;

        auto operation = Connect(
            Ops::move(task),
            CollectReceiver<T>{&value, &error, &done, ReactorScheduler(*this)}
        );
        Start(operation);

        while (!done) {
            static_cast<void>(run_once());
        }

        if (error != nullptr) {
            System::rethrow_exception(error);
        }
        return Ops::move(*value);
    }

    #endif
}

#endif
