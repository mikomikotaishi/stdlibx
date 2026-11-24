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
     * Work is posted through Reactor::post_ready, so it runs on the reactor's
     * next turn rather than re-entering the loop from inside a handler.
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
            Reactor::ReadyNode node; ///< Intrusive storage for the pending post; lives as long as this op.

            /**
             * @brief Queue the completion on the reactor's next turn.
             *
             * post_ready neither allocates nor throws, so there is no error to
             * report and no try/catch: the reactor owns no part of this node and
             * simply calls back into @ref resume when the turn comes. That is
             * exactly what makes ScheduleSender infallible, and therefore usable
             * as a coroutine Task's start scheduler.
             */
            void start() & noexcept {
                node.run = &Operation::resume;
                node.context = this;
                reactor->post_ready(&node);
            }

            /**
             * @internal
             * @brief Trampoline from the intrusive node back to the receiver.
             */
            static void resume(void* context) noexcept {
                Operation* self = static_cast<Operation*>(context);
                SetValue(Ops::move(self->receiver));
            }
        };

        /**
         * @internal
         * @struct ScheduleSender
         * @brief What @ref ReactorScheduler::schedule returns.
         *
         * Completes only with set_value: the post cannot fail. An error channel
         * here would make the scheduler fallible and disqualify it as a Task's
         * start scheduler (stdexec requires __infallible_scheduler there).
         */
        struct ScheduleSender {
            using sender_concept = SenderTag;
            using completion_signatures = CompletionSignatures<
                stdx::exec::types::SetValue()
            >;

            Reactor* reactor; ///< The reactor the work belongs to.

            template <typename Rcvr>
            [[nodiscard]]
            Operation<Rcvr> connect(Rcvr receiver) const {
                return Operation<Rcvr> {
                    .reactor = reactor,
                    .receiver = Ops::move(receiver),
                    .node = {},
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
     * @internal
     * @brief Shared operation state for a reactor retry-loop sender.
     *
     * The byte and accept operations all have one shape: try the non-blocking
     * primitive; on success complete the receiver with the result; on
     * would-block wait on @ref Readiness and retry. This captures that once, as
     * a plain sender operation state rather than a Task coroutine, so awaiting
     * one inserts no child-task frame and so no stdexec __finally affinity hop
     * (see the note by the forward declarations in reactor.inl).
     *
     * The inner Readiness operation is re-connected on each would-block through
     * a manual-lifetime slot: emplace() destroys the previous one and builds a
     * fresh one, mirroring what the old coroutine's `co_await Readiness` did
     * each loop iteration.
     *
     * @tparam T The value the operation yields.
     * @tparam Step An invocable returning Optional<T>: one non-blocking attempt.
     * @tparam Rcvr The downstream receiver.
     */
    template <typename T, typename Step, typename Rcvr>
    struct RetryOperation {
        using operation_state_concept = OperationStateTag;

        Reactor* reactor; ///< The loop the readiness waits run on.
        Socket::NativeHandle handle; ///< The descriptor to wait on when blocked.
        Interest interest; ///< What readiness to wait for (READ or WRITE).
        Step step; ///< One non-blocking attempt; returns a value or nullopt.
        Rcvr receiver; ///< Completed with the final value or an error.

        /**
         * @internal
         * @brief Receiver for the inner Readiness: a readiness wake retries.
         */
        struct Retry {
            using receiver_concept = ReceiverTag;
            RetryOperation* self; ///< The operation to drive forward.

            void set_value() noexcept {
                self->attempt();
            }
            void set_error(ExceptionPointer error) noexcept {
                SetError(Ops::move(self->receiver), Ops::move(error));
            }
            void set_stopped() noexcept {
                SetStopped(Ops::move(self->receiver));
            }
        };

        using Inner = stdx::exec::ConnectResult<Readiness, Retry>;
        Optional<Inner> inner; ///< The in-flight readiness wait, re-armed each retry.

        void start() & noexcept {
            attempt();
        }

        /**
         * @internal
         * @brief One attempt: complete on success, else wait and retry.
         */
        void attempt() noexcept {
            try {
                if (Optional<T> result = step(); result.has_value()) {
                    SetValue(Ops::move(receiver), Ops::move(*result));
                    return;
                }
                inner.emplace(Connect(Readiness(*reactor, handle, interest), Retry{this}));
                Start(*inner);
            } catch (...) {
                SetError(Ops::move(receiver), System::current_exception());
            }
        }
    };

    /**
     * @class ReceiveSender
     * @brief A sender that reads from a stream, waiting until it has data.
     */
    template <typename S>
    class [[nodiscard]] ReceiveSender {
    public:
        using sender_concept = SenderTag;
        using completion_signatures = CompletionSignatures<
            stdx::exec::types::SetValue(usize),
            stdx::exec::types::SetError(ExceptionPointer)
        >;

        Reactor* reactor; ///< The loop to wait on.
        S* stream; ///< The byte reader; not owned.
        Span<byte> into; ///< Where read bytes go.

        template <typename Rcvr>
        [[nodiscard]]
        auto connect(Rcvr receiver) const {
            S* s = stream;
            Span<byte> buf = into;
            auto step = [s, buf]() -> Optional<usize> { return s->try_receive(buf); };
            return RetryOperation<usize, decltype(step), Rcvr>{
                reactor, stream->native_handle(), Interest::READ,
                Ops::move(step), Ops::move(receiver), nullopt,
            };
        }
    };

    /**
     * @class SendSender
     * @brief A sender that writes to a stream, waiting until it will take bytes.
     */
    template <typename S>
    class [[nodiscard]] SendSender {
    public:
        using sender_concept = SenderTag;
        using completion_signatures = CompletionSignatures<
            stdx::exec::types::SetValue(usize),
            stdx::exec::types::SetError(ExceptionPointer)
        >;

        Reactor* reactor;
        S* stream;
        Span<const byte> from;

        template <typename Rcvr>
        [[nodiscard]]
        auto connect(Rcvr receiver) const {
            S* s = stream;
            Span<const byte> buf = from;
            auto step = [s, buf]() -> Optional<usize> { return s->try_send(buf); };
            return RetryOperation<usize, decltype(step), Rcvr>{
                reactor, stream->native_handle(), Interest::WRITE,
                Ops::move(step), Ops::move(receiver), nullopt,
            };
        }
    };

    /**
     * @internal
     * @brief Operation state for SendAllSender: write everything, re-arming on
     * each short write, as a flat operation (no nested send sender).
     */
    template <typename S, typename Rcvr>
    struct SendAllOperation {
        using operation_state_concept = OperationStateTag;

        Reactor* reactor;
        S* stream;
        Span<const byte> from;
        usize total = 0; ///< Bytes written so far.
        Rcvr receiver;

        struct Retry {
            using receiver_concept = ReceiverTag;
            SendAllOperation* self;
            void set_value() noexcept { self->pump(); }
            void set_error(ExceptionPointer error) noexcept {
                SetError(Ops::move(self->receiver), Ops::move(error));
            }
            void set_stopped() noexcept { SetStopped(Ops::move(self->receiver)); }
        };

        using Inner = stdx::exec::ConnectResult<Readiness, Retry>;
        Optional<Inner> inner;

        void start() & noexcept { pump(); }

        void pump() noexcept {
            try {
                while (total < from.size()) {
                    const Optional<usize> written = stream->try_send(from.subspan(total));
                    if (!written.has_value()) {
                        // Would block: wait for writability, then resume pumping.
                        inner.emplace(Connect(
                            Readiness(*reactor, stream->native_handle(), Interest::WRITE),
                            Retry{this}
                        ));
                        Start(*inner);
                        return;
                    }
                    if (*written == 0) {
                        break; // End of stream; stop short.
                    }
                    total += *written;
                }
                SetValue(Ops::move(receiver), total);
            } catch (...) {
                SetError(Ops::move(receiver), System::current_exception());
            }
        }
    };

    /**
     * @class SendAllSender
     * @brief A sender that writes all of a buffer, waiting as often as needed.
     */
    template <typename S>
    class [[nodiscard]] SendAllSender {
    public:
        using sender_concept = SenderTag;
        using completion_signatures = CompletionSignatures<
            stdx::exec::types::SetValue(usize),
            stdx::exec::types::SetError(ExceptionPointer)
        >;

        Reactor* reactor;
        S* stream;
        Span<const byte> from;

        template <typename Rcvr>
        [[nodiscard]]
        auto connect(Rcvr receiver) const {
            return SendAllOperation<S, Rcvr>{
                reactor, stream, from, 0, Ops::move(receiver), nullopt,
            };
        }
    };

    /**
     * @class AcceptSender
     * @brief A sender that accepts one connection, waiting for a peer.
     */
    template <typename L>
    class [[nodiscard]] AcceptSender {
    public:
        using Stream = typename L::Stream;
        using sender_concept = SenderTag;
        using completion_signatures = CompletionSignatures<
            stdx::exec::types::SetValue(Stream),
            stdx::exec::types::SetError(ExceptionPointer)
        >;

        Reactor* reactor;
        L* listener; ///< The acceptor; not owned.

        template <typename Rcvr>
        [[nodiscard]]
        auto connect(Rcvr receiver) const {
            L* l = listener;
            auto step = [l]() -> Optional<Stream> { return l->try_accept(); };
            return RetryOperation<Stream, decltype(step), Rcvr>{
                reactor, listener->native_handle(), Interest::READ,
                Ops::move(step), Ops::move(receiver), nullopt,
            };
        }
    };

    /**
     * @brief Reads from @p stream, suspending until it has something to give.
     */
    template <ByteReader S>
    ReceiveSender<S> Reactor::receive(S& stream, Span<byte> into) noexcept {
        return ReceiveSender<S>{this, &stream, into};
    }

    /**
     * @brief Writes to @p stream, suspending until it will take bytes.
     */
    template <ByteWriter S>
    SendSender<S> Reactor::send(S& stream, Span<const byte> from) noexcept {
        return SendSender<S>{this, &stream, from};
    }

    /**
     * @brief Writes all of @p from, suspending as often as it takes.
     */
    template <ByteWriter S>
    SendAllSender<S> Reactor::send_all(S& stream, Span<const byte> from) noexcept {
        return SendAllSender<S>{this, &stream, from};
    }

    /**
     * @brief Accepts one connection, suspending until a peer arrives.
     */
    template <Acceptor L>
    AcceptSender<L> Reactor::accept(L& listener) noexcept {
        return AcceptSender<L>{this, &listener};
    }

    #ifdef __linux__
    /**
     * @internal
     * @brief C-linkage view of glibc's pidfd_open.
     *
     * The call cannot go through the module's re-exported stdx::os::linux::sys::
     * pidfd_open: a C function re-exported via `using ::name` through a C++20
     * module is mangled with C++ linkage at the call site under current
     * toolchains, producing an undefined reference to a name the C library does
     * not define (the real symbol is the C `pidfd_open@@GLIBC_2.36`). A local
     * extern "C" redeclaration names that symbol directly. glibc declares it as
     * `int pidfd_open(pid_t, unsigned int)`.
     */
    extern "C" int pidfd_open(unix::ProcessId __pid, unsigned int __flags) noexcept;

    /**
     * @brief Spawn a child bound to this Reactor so its exit can be awaited.
     *
     * Opens a pidfd for the freshly spawned child and hands it to the Process.
     * A pidfd becomes readable exactly when the child it names terminates, so
     * the reactor can watch it like any other descriptor and @ref Readiness
     * does the rest. If pidfd_open fails the child is already running and must
     * not be leaked, so it is returned as an ordinary (non-awaitable) Process
     * rather than discarded - the caller can still wait() on it synchronously.
     */
    inline Expected<stdx::sys::Process, ErrorCode> Reactor::spawn(stdx::sys::Process::Builder builder) {
        Expected<stdx::sys::Process, ErrorCode> child = builder.spawn();
        if (!child) {
            return child;
        }
        const i32 pidfd = static_cast<i32>(
            stdx::net::pidfd_open(static_cast<unix::ProcessId>(child->id()), 0)
        );
        if (pidfd == -1) {
            // The child exists and is running; hand it back un-bound so it is
            // not leaked. wait_async would reject it, but wait() still works.
            return child;
        }
        child->set_exit_pidfd(pidfd);
        return child;
    }
    #endif
}

#ifdef __linux__
/**
 * @namespace stdx::sys
 * @brief Out-of-line definition of Process::wait_async, which needs the
 * complete Reactor and Readiness types from this header.
 */
namespace stdx::sys {
    /**
     * @brief Asynchronously wait for the child to exit. @see declaration.
     *
     * Suspends on the child's pidfd until the reactor reports it readable - a
     * pidfd signals read-readiness on child exit - then reaps with a
     * non-blocking waitpid to collect the real exit status. The pidfd stays
     * open and owned by the Process; closing it is the Process's job.
     */
    inline ReactorTask<ExitStatus> Process::wait_async(stdx::net::Reactor& reactor) {
        // Precondition: the Process must have been spawned via Reactor::spawn.
        if (_exit_pidfd == -1) {
            throw stdx::net::ReactorException(
                "Process::wait_async requires a Process spawned via Reactor::spawn"
            );
        }
        if (_pid == -1) {
            throw RuntimeException("Process::wait_async called on an already-reaped Process");
        }

        // A readable pidfd means the child has exited. One wait is enough: a
        // pidfd does not go readable and then clear, so there is no spurious
        // wakeup to loop around the way the socket operations must.
        co_await stdx::net::Readiness(reactor, _exit_pidfd, stdx::net::Interest::READ);

        i32 status = 0;
        unix::ProcessId ret;
        do {
            ret = unix::sys::waitpid(_pid, &status, 0);
        } while (ret == -1 && unix::errnov() == unix::EINTR);
        if (ret == -1) {
            throw RuntimeException("Process::wait_async: waitpid failed after pidfd readiness");
        }
        _pid = -1;
        co_return ExitStatus(status);
    }
}
#endif

namespace stdx::net {
    // ---------------------------------------------------------------------
    // Reactor::run_until drives a Reactor::Task<T> (Task<T, Reactor::Env>), not a
    // plain Task. Three things had to line up, discovered in this order:
    //
    //   1. The receiver environment must answer get_start_scheduler, not only
    //      get_scheduler. ReactorReceiverEnv below answers both; the query type is
    //      exported from stdx::exec as GetStartScheduler (src/stdx/exec.cppm).
    //   2. The task's start scheduler must be infallible: its schedule() sender
    //      may complete only with set_value. ReactorScheduler::schedule() posts
    //      via Reactor::post_ready, a non-allocating noexcept enqueue, so it has
    //      no error channel - unlike the old Reactor::after path, which could
    //      throw on timer-slot allocation.
    //   3. The task must NOT use the default environment. With Task<T, Env<>>,
    //      stdexec defaults the start scheduler to task_scheduler and type-erases
    //      ReactorScheduler into it. Awaiting a value-producing operation then
    //      copied that type-erased backend through a __value_proxy_root whose
    //      teardown dereferenced a null dispatch pointer - a hard SEGV in
    //      ~__value_proxy_root<__itask_scheduler_backend> reached from the task
    //      promise's await_transform. Reactor::Env names ReactorScheduler as
    //      the start_scheduler_type directly, so the task holds the concrete
    //      scheduler and the type-erased proxy (and its crashing teardown) never
    //      exists. This is why every reactor coroutine returns Reactor::Task<T>,
    //      and why awaiting a plain Task<T> inside one must be avoided - it would
    //      reintroduce the default-env child and the same crash.
    //
    // Earlier notes blamed an stdexec __finally bug and then a reactor
    // reentrancy bug; both were wrong. The cause was (3): our own use of the
    // type-erased task_scheduler. The byte/accept operations were also moved
    // from Task coroutines to plain senders along the way, which is good
    // regardless (no per-operation child-task frame), but (3) is what fixed it.
    // ---------------------------------------------------------------------
    #ifdef STDLIBX_NET_ASYNC_TASKS
    /**
     * @internal
     * @struct ReactorReceiverEnv
     * @brief The receiver environment that hands a Task its scheduler.
     */
    struct ReactorReceiverEnv {
        ReactorScheduler scheduler; ///< Where continuations are resumed.

        /**
         * @brief The scheduler a Task starts (and resumes) on.
         *
         * The coroutine Task queries get_start_scheduler to build the scheduler
         * it pins itself to; answering it is half of what run_until needs. The
         * other half is that ReactorScheduler::schedule() is infallible, which
         * stdexec requires of any task start scheduler.
         */
        [[nodiscard]]
        ReactorScheduler query(stdx::exec::types::GetStartScheduler) const noexcept {
            return scheduler;
        }

        /**
         * @brief The scheduler ordinary get_scheduler lookups resolve to.
         *
         * Kept alongside get_start_scheduler so senders awaited inside the Task
         * that ask for a plain scheduler still find the reactor's.
         */
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
        ReactorReceiverEnv get_env() const noexcept {
            return ReactorReceiverEnv {
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
    T Reactor::run_until(Reactor::Task<T> task) {
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
