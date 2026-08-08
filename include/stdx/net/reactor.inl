#pragma once

using stdx::collections::Deque;
using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::sync::Atomic;
using stdx::time::Instant;
using stdx::time::Milliseconds;
using stdx::time::SteadyClock;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::Task;
#endif

namespace stdx::net {
    #ifdef __cpp_lib_move_only_function
    template <typename Sig>
    using OwnedCallable = MoveOnlyFunction<Sig>;
    #else
    template <typename Sig>
    using OwnedCallable = Function<Sig>;
    #endif
}

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    class Readiness;
    #endif

    /**
     * @class ReactorException
     * @brief Exception class for handling event-loop misuse.
     * @extends IOException
     *
     * Distinct from PollerException, which reports the operating system
     * refusing a readiness operation. A ReactorException means the loop was
     * asked to do something incoherent - registering a descriptor twice,
     * running a loop that is already running - and so names a caller mistake
     * rather than an environmental failure.
     */
    class ReactorException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @class TimerId
     * @brief A handle to one scheduled callback, for cancelling it.
     *
     * A default-constructed id is valid to hold and never names anything.
     */
    class [[nodiscard]] TimerId {
    private:
        friend class Reactor;

        u64 _value = 0; ///< The slot index biased by one in the high half, the generation in the low half.

        constexpr explicit TimerId(u64 value) noexcept:
            _value{value} {}
    public:
        constexpr TimerId() noexcept = default;

        /**
         * @brief Whether this id was issued by a Reactor.
         * @return false for a default-constructed id, true otherwise.
         *
         * A true answer does not mean the timer is still pending: it may have
         * fired or been cancelled since. Only Reactor::cancel can tell.
         */
        [[nodiscard]]
        constexpr bool is_valid() const noexcept {
            return _value != 0;
        }

        [[nodiscard]]
        friend constexpr bool operator==(TimerId left, TimerId right) noexcept = default;
    };

    /**
     * @class Reactor
     * @brief An event loop: readiness and deadlines, dispatched to callbacks.
     *
     * A Poller answers "which of these descriptors are ready"; a Reactor answers
     * "run this when it is". It owns a Poller, keeps the callback that belongs
     * with each descriptor, and folds timers into the same wait, so a loop that
     * needs both costs one blocking call rather than a poll and a sleep.
     *
     * Three things it takes off the caller:
     *
     * - **Tokens.** Poller::add wants a u64 to hand back, and the caller has to
     *   invent one that finds the connection again. A Reactor issues its own and
     *   keeps the handler beside it, so registration takes the callback directly.
     * - **Stale events.** A token carries the generation of the slot that issued
     *   it. A descriptor removed earlier in the same turn cannot have its handler
     *   run by an event already collected, which is the failure the token scheme
     *   would otherwise make invisible.
     * - **Deadlines.** @ref after and @ref at go in a heap whose earliest entry
     *   sets the wait timeout, so timers cost no syscall of their own.
     *
     * Handlers may register, re-register, cancel and remove freely, including
     * removing the descriptor they were called for. A slot removed during
     * dispatch is deregistered from the kernel at once, so the descriptor may be
     * closed immediately, but its handler object is not destroyed until the turn
     * ends - a handler that removes itself keeps running to its `return`.
     *
     * As with Poller, the reactor never owns the descriptors it watches, and the
     * rule stays: **remove a descriptor before closing it.**
     *
     * Not thread-safe, with the same single exception Poller carves out:
     * @ref stop may be called from any thread.
     */
    class [[nodiscard]] Reactor {
    public:
        using NativeHandle = Socket::NativeHandle; ///< The platform's descriptor type, matching the one Socket hands out.
        using Handler = OwnedCallable<void(const Event&)>; ///< What runs when a descriptor is ready.
        using Callback = OwnedCallable<void()>; ///< What runs when a deadline passes.
    private:
        /**
         * @internal
         * @struct Slot
         * @brief One registered descriptor: its handler, and what it is watched for.
         */
        struct Slot {
            Handler handler; ///< What to run; empty exactly when the slot is free.
            NativeHandle handle = Socket::INVALID_HANDLE; ///< The descriptor, for removing it again.
            u32 generation = 0; ///< Bumped on release, so a stale token is recognised.
            Interest interest = Interest::READ; ///< What it is currently registered for.
            bool alive = false; ///< Whether a caller currently has this descriptor registered.
        };

        /**
         * @internal
         * @struct Timer
         * @brief One scheduled callback.
         */
        struct Timer {
            Callback callback; ///< What to run; empty exactly when the slot is free.
            u32 generation = 0; ///< Bumped on release, so a cancelled id cannot fire.
            bool alive = false; ///< Whether the timer is still pending.
        };

        /**
         * @internal
         * @struct TimerNode
         * @brief A timer's place in the deadline heap.
         *
         * Deliberately a copy of the key rather than a pointer into @ref _timers:
         * the heap is reordered constantly and the slab is not, so keeping them
         * apart means a cancelled timer need not be hunted down in the heap. It
         * is left to surface at the top and be discarded on the generation check.
         */
        struct TimerNode {
            Instant<SteadyClock> deadline; ///< When the callback comes due.
            u32 index = 0; ///< Which slab entry it belongs to.
            u32 generation = 0; ///< The generation that scheduled it.
        };

        Poller _poller; ///< The readiness mechanism the loop is built on.

        Deque<Slot> _slots; ///< Every descriptor slot ever allocated, live or free.
        Vector<u32> _free_slots; ///< Indices of the free entries of @ref _slots.
        HashMap<NativeHandle, u32> _handles; ///< Which slot each registered descriptor occupies.

        Deque<Timer> _timers; ///< Every timer slot ever allocated, live or free.
        Vector<u32> _free_timers; ///< Indices of the free entries of @ref _timers.
        Vector<TimerNode> _heap; ///< The deadlines, as a min-heap on TimerNode::deadline.
        usize _live_timers = 0; ///< How many entries of @ref _timers are pending.

        Vector<u32> _pending_release; ///< Slots removed mid-dispatch, to be freed when the turn ends.
        Vector<TimerNode> _expired; ///< The deadlines that came due this turn, taken off the heap before any fires.

        bool _dispatching = false; ///< Whether a turn is in progress, so removal defers.
        Atomic<bool> _stopping{false}; ///< Set by @ref stop from any thread; read by @ref run.
        bool _running = false; ///< Whether @ref run is on the stack, so re-entry can be refused.

        /**
         * @internal
         * @brief The token standing for slot @p index at @p generation.
         * @param index The slot index.
         * @param generation The slot's generation at the time of registration.
         * @return The token to hand to the Poller.
         */
        [[nodiscard]]
        static constexpr u64 make_token(u32 index, u32 generation) noexcept {
            return (static_cast<u64>(index) << 32) | static_cast<u64>(generation);
        }

        /**
         * @internal
         * @brief The slot index a token names.
         * @param token The token to extract the index from.
         * @return The slot index of the token.
         */
        [[nodiscard]]
        static constexpr u32 token_index(u64 token) noexcept {
            return static_cast<u32>(token >> 32);
        }

        /**
         * @internal
         * @brief The generation a token was issued at.
         * @param token The token to extract the generation from.
         * @return The generation of the token.
         */
        [[nodiscard]]
        static constexpr u32 token_generation(u64 token) noexcept {
            return static_cast<u32>(token);
        }

        /**
         * @internal
         * @brief Takes a free slot index, growing the slab if there is none.
         * @return The index of a slot that is not alive.
         */
        [[nodiscard]]
        u32 acquire_slot() {
            if (!_free_slots.empty()) {
                const u32 index = _free_slots.back();
                _free_slots.pop_back();
                return index;
            }
            _slots.emplace_back();
            return static_cast<u32>(_slots.size() - 1);
        }

        /**
         * @internal
         * @brief Marks slot @p index free and drops its handler.
         * @param index The slot to release.
         *
         * Bumping the generation first is what makes an already-collected event
         * for this slot harmless: the token it carries no longer matches.
         */
        void release_slot(u32 index) noexcept {
            Slot& slot = _slots[index];
            slot.handler = nullptr;
            slot.handle = Socket::INVALID_HANDLE;
            slot.alive = false;
            ++slot.generation;
            _free_slots.push_back(index);
        }

        /**
         * @internal
         * @brief Frees the slots whose removal was deferred past the dispatch turn.
         */
        void drain_pending_release() noexcept {
            for (const u32 index: _pending_release) {
                release_slot(index);
            }
            _pending_release.clear();
        }

        /**
         * @internal
         * @brief Restores the heap by moving the last entry up to its place.
         * @param child The index of the last entry to sift up.
         */
        void heap_sift_up(usize child) noexcept {
            while (child > 0) {
                const usize parent = (child - 1) / 2;
                if (!(_heap[child].deadline < _heap[parent].deadline)) {
                    return;
                }
                const TimerNode moved = _heap[parent];
                _heap[parent] = _heap[child];
                _heap[child] = moved;
                child = parent;
            }
        }

        /**
         * @internal
         * @brief Restores the heap by moving the root entry down to its place.
         * @param parent The index of the root to sift down.
         */
        void heap_sift_down(usize parent) noexcept {
            const usize count = _heap.size();
            while (true) {
                const usize left = parent * 2 + 1;
                if (left >= count) {
                    return;
                }
                const usize right = left + 1;
                usize smallest = left;
                if (right < count && _heap[right].deadline < _heap[left].deadline) {
                    smallest = right;
                }
                if (!(_heap[smallest].deadline < _heap[parent].deadline)) {
                    return;
                }
                const TimerNode moved = _heap[parent];
                _heap[parent] = _heap[smallest];
                _heap[smallest] = moved;
                parent = smallest;
            }
        }

        /**
         * @internal
         * @brief Adds @p node to the deadline heap.
         * @param node The timer to add.
         */
        void heap_push(TimerNode node) {
            _heap.push_back(node);
            heap_sift_up(_heap.size() - 1);
        }

        /**
         * @internal
         * @brief Removes the earliest deadline from the heap.
         */
        void heap_pop() noexcept {
            _heap[0] = _heap.back();
            _heap.pop_back();
            if (!_heap.empty()) {
                heap_sift_down(0);
            }
        }

        /**
         * @internal
         * @brief Discards heap entries whose timers have been cancelled.
         *
         * Cancellation does not touch the heap, so the root may name a timer
         * that no longer exists. Clearing those before the root is read keeps
         * @ref next_deadline honest and stops a cancelled timer from holding the
         * wait to a deadline nobody is waiting for.
         */
        void drop_cancelled_roots() noexcept {
            while (!_heap.empty()) {
                const TimerNode& node = _heap.front();
                const Timer& timer = _timers[node.index];
                if (timer.alive && timer.generation == node.generation) {
                    return;
                }
                heap_pop();
            }
        }

        /**
         * @internal
         * @brief How long the wait may block, given the timers and @p timeout.
         * @param timeout The caller's own limit, or an empty Optional for none.
         * @return The shorter of the two, or an empty Optional if neither applies.
         */
        [[nodiscard]]
        Optional<Milliseconds> effective_timeout(const Optional<Milliseconds>& timeout) {
            drop_cancelled_roots();
            if (_heap.empty()) {
                return timeout;
            }

            const Instant<SteadyClock> now = SteadyClock::now();
            const Instant<SteadyClock> deadline = _heap.front().deadline;
            Milliseconds until = 0ms;
            if (now < deadline) {
                // Rounding up matters: a deadline 1500us out truncates to 1ms,
                // the wait returns early, the timer is still not due, and the
                // loop spins. Waking a fraction late costs nothing by contrast.
                const Nanoseconds remaining = stdx::time::duration_cast<Nanoseconds>(deadline - now);
                until = stdx::time::duration_cast<Milliseconds>(remaining + 999999ns);
            }
            if (timeout.has_value() && *timeout < until) {
                return timeout;
            }
            return until;
        }

        /**
         * @internal
         * @brief Moves every due timer off the heap, before any of them runs.
         * @return Whether anything came due.
         *
         * Taking them all first is what makes a callback free to schedule
         * another timer: a callback that rescheduled itself while the heap was
         * still being drained would otherwise be re-collected in the same turn
         * and run forever without the loop ever blocking again.
         */
        [[nodiscard]]
        bool collect_expired() {
            _expired.clear();
            const Instant<SteadyClock> now = SteadyClock::now();
            while (true) {
                drop_cancelled_roots();
                if (_heap.empty() || now < _heap.front().deadline) {
                    break;
                }
                _expired.push_back(_heap.front());
                heap_pop();
            }

            for (const TimerNode& node: _expired) {
                Timer& timer = _timers[node.index];
                timer.alive = false;
                ++timer.generation;
                --_live_timers;
            }
            return !_expired.empty();
        }

        /**
         * @internal
         * @brief Runs the callbacks collected by @ref collect_expired.
         */
        void fire_expired() {
            for (const TimerNode& node: _expired) {
                Timer& timer = _timers[node.index];
                Callback callback = Ops::move(timer.callback);
                timer.callback = nullptr;
                _free_timers.push_back(node.index);
                callback();
            }
            _expired.clear();
        }

        /**
         * @internal
         * @brief Runs the handler for each event, skipping the ones since removed.
         * @param events The events the Poller collected.
         */
        void dispatch(Span<const Event> events) {
            for (const Event& event: events) {
                const u32 index = token_index(event.token);
                if (index >= _slots.size()) {
                    continue;
                }
                Slot& slot = _slots[index];
                if (!slot.alive || slot.generation != token_generation(event.token)) {
                    continue;
                }
                slot.handler(event);
            }
        }
    public:
        /**
         * @brief Constructs a reactor with nothing registered and no timers.
         * @throws PollerException if the readiness mechanism cannot be created.
         */
        THROWS(PollerException)
        Reactor() = default;

        Reactor(const Reactor&) = DELETE_METHOD("Reactor owns its Poller and cannot be copied.");

        Reactor& operator=(const Reactor&) = DELETE_METHOD("Reactor owns its Poller and cannot be copied.");

        Reactor(Reactor&& other) noexcept:
            _poller{Ops::move(other._poller)},
            _slots{Ops::move(other._slots)},
            _free_slots{Ops::move(other._free_slots)},
            _handles{Ops::move(other._handles)},
            _timers{Ops::move(other._timers)},
            _free_timers{Ops::move(other._free_timers)},
            _heap{Ops::move(other._heap)},
            _live_timers{other._live_timers},
            _pending_release{Ops::move(other._pending_release)},
            _expired{Ops::move(other._expired)},
            _stopping{other._stopping.load()} {
            other._live_timers = 0;
        }

        Reactor& operator=(Reactor&& other) noexcept {
            if (this != &other) {
                _poller = Ops::move(other._poller);
                _slots = Ops::move(other._slots);
                _free_slots = Ops::move(other._free_slots);
                _handles = Ops::move(other._handles);
                _timers = Ops::move(other._timers);
                _free_timers = Ops::move(other._free_timers);
                _heap = Ops::move(other._heap);
                _live_timers = other._live_timers;
                _pending_release = Ops::move(other._pending_release);
                _expired = Ops::move(other._expired);
                _stopping.store(other._stopping.load());
                other._live_timers = 0;
            }
            return *this;
        }

        ~Reactor() = default;

        /**
         * @brief Registers @p handle, calling @p handler whenever it is ready.
         * @param handle The descriptor to watch; the Reactor does not take ownership of it.
         * @param interest The readiness to be told about.
         * @param handler What to run for each Event on this descriptor.
         * @throws ReactorException if @p handle is already registered.
         * @throws PollerException if the operating system refuses the registration.
         *
         * The handler is called with the Event as the loop collected it, and may
         * do anything to the reactor, including removing @p handle or itself.
         */
        THROWS(ReactorException, PollerException)
        void add(NativeHandle handle, Interest interest, Handler handler) {
            if (_handles.contains(handle)) {
                throw ReactorException("descriptor is already registered");
            }

            const u32 index = acquire_slot();
            Slot& slot = _slots[index];
            try {
                _poller.add(handle, interest, make_token(index, slot.generation));
            } catch (...) {
                _free_slots.push_back(index);
                throw;
            }

            slot.handler = Ops::move(handler);
            slot.handle = handle;
            slot.interest = interest;
            slot.alive = true;
            _handles.emplace(handle, index);
        }

        /**
         * @brief Changes what @p handle is reported on, keeping its handler.
         * @param handle A descriptor already passed to @ref add.
         * @param interest The readiness to be told about from now on.
         * @throws ReactorException if @p handle is not registered.
         * @throws PollerException if the operating system refuses the change.
         *
         * The usual reason to call this is a write that went short: add WRITE
         * until the rest of the buffer drains, then drop back to READ.
         */
        THROWS(ReactorException, PollerException)
        void modify(NativeHandle handle, Interest interest) {
            const auto found = _handles.find(handle);
            if (found == _handles.end()) {
                throw ReactorException("descriptor is not registered");
            }

            const u32 index = found->second;
            Slot& slot = _slots[index];
            _poller.modify(handle, interest, make_token(index, slot.generation));
            slot.interest = interest;
        }

        /**
         * @brief Replaces the handler for @p handle, keeping its interest.
         * @param handle A descriptor already passed to @ref add.
         * @param handler What to run from now on.
         * @throws ReactorException if @p handle is not registered.
         *
         * Handing a connection from one state machine to the next - a request
         * parser to a response writer, a plaintext handler to a TLS one - is a
         * handler swap, not a re-registration, and does not touch the kernel.
         */
        THROWS(ReactorException)
        void set_handler(NativeHandle handle, Handler handler) {
            const auto found = _handles.find(handle);
            if (found == _handles.end()) {
                throw ReactorException("descriptor is not registered");
            }
            _slots[found->second].handler = Ops::move(handler);
        }

        /**
         * @brief Stops reporting on @p handle.
         * @param handle A descriptor already passed to @ref add.
         * @throws ReactorException if @p handle is not registered.
         * @throws PollerException if the operating system refuses the removal.
         *
         * The kernel registration is dropped before this returns, so the
         * descriptor may be closed immediately afterwards. When called from
         * inside a handler the handler object outlives the call and is destroyed
         * once the turn ends, which is what lets a handler remove itself.
         */
        THROWS(ReactorException, PollerException)
        void remove(NativeHandle handle) {
            const auto found = _handles.find(handle);
            if (found == _handles.end()) {
                throw ReactorException("descriptor is not registered");
            }

            const u32 index = found->second;
            _poller.remove(handle);
            _handles.erase(found);

            Slot& slot = _slots[index];
            slot.alive = false;
            slot.handle = Socket::INVALID_HANDLE;
            if (_dispatching) {
                // The generation moves now, so any event already collected for
                // this slot is ignored for the rest of the turn; only the
                // handler's destruction waits.
                ++slot.generation;
                _pending_release.push_back(index);
            } else {
                release_slot(index);
            }
        }

        /**
         * @brief Whether @p handle is currently registered.
         * @param handle The descriptor to check.
         * @return true if it is registered, false otherwise.
         */
        [[nodiscard]]
        bool watches(NativeHandle handle) const noexcept {
            return _handles.contains(handle);
        }

        /**
         * @brief How many descriptors are registered.
         * @return The count of descriptors the Reactor is watching.
         */
        [[nodiscard]]
        usize registered() const noexcept {
            return _handles.size();
        }

        #ifdef STDLIBX_EXECUTION_AVAILABLE
        /**
         * @brief A sender that completes once @p handle has something to read.
         * @param handle The descriptor to wait on; not owned.
         * @return A sender that completes when @p handle is readable.
         */
        [[nodiscard]]
        Readiness readable(NativeHandle handle) noexcept;

        /**
         * @brief A sender that completes once @p handle will accept a write.
         * @param handle The descriptor to wait on; not owned.
         * @return A sender that completes when @p handle is writable.
         */
        [[nodiscard]]
        Readiness writable(NativeHandle handle) noexcept;

        /**
         * @brief Reads from @p stream, suspending until it has something to give.
         * @param stream Any ByteReader - a TcpStream, a TlsStream, or a test double.
         * @param into Where to put the bytes.
         * @return How many bytes were read; 0 at end of stream.
         */
        template <ByteReader S>
        [[nodiscard]]
        Task<usize> receive(S& stream, Span<byte> into);

        /**
         * @brief Writes to @p stream, suspending until it will take bytes.
         * @param stream Any ByteWriter.
         * @param from The bytes to write.
         * @return How many bytes were accepted, which may be fewer than offered.
         */
        template <ByteWriter S>
        [[nodiscard]]
        Task<usize> send(S& stream, Span<const byte> from);

        /**
         * @brief Writes all of @p from, suspending as often as it takes.
         * @param stream Any ByteWriter.
         * @param from The bytes to write.
         * @return How many bytes were written; less than the span only at end of stream.
         */
        template <ByteWriter S>
        [[nodiscard]]
        Task<usize> send_all(S& stream, Span<const byte> from);

        /**
         * @brief Accepts one connection, suspending until a peer arrives.
         * @param listener A bound and listening Acceptor.
         * @return The accepted connection, of whatever type @p listener yields.
         */
        template <Acceptor L>
        [[nodiscard]]
        Task<typename L::Stream> accept(L& listener);

        #ifdef STDLIBX_NET_ASYNC_TASKS
        /**
         * @brief Runs @p task to completion, driving this Reactor until it finishes.
         * @param task The work to run.
         * @return The task's value.
         * @throws whatever @p task threw.
         *
         * The entry point from ordinary code into the asynchronous half, and the
         * third member of the @ref run_once / @ref run family: everything the
         * task awaits is resumed on this thread, from inside this loop, which is
         * what keeps the not-thread-safe Reactor safe to touch.
         */
        template <typename T>
        T run_until(Task<T> task);
        #endif
        #endif

        /**
         * @brief Runs @p callback once, @p delay from now.
         * @param delay How long to wait; zero or negative comes due at the next turn.
         * @param callback What to run.
         * @return The id to @ref cancel it with.
         */
        [[nodiscard]]
        TimerId after(Milliseconds delay, Callback callback) {
            return at(SteadyClock::now() + delay, Ops::move(callback));
        }

        /**
         * @brief Runs @p callback once, at @p deadline.
         * @param deadline When to run it; a deadline in the past comes due at the next turn.
         * @param callback What to run.
         * @return The id to @ref cancel it with.
         *
         * Timers are on the steady clock, so a wall-clock adjustment cannot make
         * a five second timeout wait an hour or fire at once.
         */
        [[nodiscard]]
        TimerId at(Instant<SteadyClock> deadline, Callback callback) {
            u32 index = 0;
            if (!_free_timers.empty()) {
                index = _free_timers.back();
                _free_timers.pop_back();
            } else {
                _timers.emplace_back();
                index = static_cast<u32>(_timers.size() - 1);
            }

            Timer& timer = _timers[index];
            timer.callback = Ops::move(callback);
            timer.alive = true;
            ++_live_timers;

            heap_push(TimerNode{deadline, index, timer.generation});
            return TimerId(make_token(index + 1, timer.generation));
        }

        /**
         * @brief Cancels a pending timer.
         * @param id The id @ref after or @ref at returned.
         * @return true if it was pending and is now cancelled, false otherwise.
         *
         * Cancelling an id that has already fired, has already been cancelled,
         * or was never issued is not an error and answers false. That is the
         * behaviour a timeout wants: the ordinary path is to cancel a read
         * timeout on arrival without first proving it has not just expired.
         */
        bool cancel(TimerId id) noexcept {
            if (!id.is_valid()) {
                return false;
            }
            const u32 index = token_index(id._value) - 1;
            if (index >= _timers.size()) {
                return false;
            }

            Timer& timer = _timers[index];
            if (!timer.alive || timer.generation != token_generation(id._value)) {
                return false;
            }

            timer.alive = false;
            timer.callback = nullptr;
            ++timer.generation;
            --_live_timers;
            _free_timers.push_back(index);
            return true;
        }

        /**
         * @brief How many timers are pending.
         * @return The count of timers that have not yet fired or been cancelled.
         */
        [[nodiscard]]
        usize pending_timers() const noexcept {
            return _live_timers;
        }

        /**
         * @brief Waits once, then runs whatever came due.
         * @param timeout A limit of the caller's own, or an empty Optional for none.
         * @return Whether any handler or callback ran.
         * @throws ReactorException if called from inside a handler.
         * @throws PollerException if the wait itself fails.
         *
         * Timers fire before I/O handlers. A turn that has both is one where the
         * deadline was already past when the descriptor became ready, and a
         * timeout that has expired should not be undone by the data that raced
         * it.
         *
         * An answer of false is ordinary: a wait can end on a signal, on
         * @ref stop, or on a timeout with nothing due.
         */
        THROWS(ReactorException, PollerException)
        bool run_once(Optional<Milliseconds> timeout = nullopt) {
            if (_dispatching) {
                throw ReactorException("the loop is already dispatching");
            }

            const Span<const Event> events = _poller.wait(effective_timeout(timeout));
            const bool timers_due = collect_expired();
            const bool anything = timers_due || !events.empty();

            _dispatching = true;
            try {
                fire_expired();
                dispatch(events);
            } catch (...) {
                _dispatching = false;
                drain_pending_release();
                throw;
            }
            _dispatching = false;
            drain_pending_release();
            return anything;
        }

        /**
         * @brief Waits and dispatches until stopped, or until nothing is left to wait for.
         * @throws ReactorException if the loop is already running.
         * @throws PollerException if a wait fails.
         *
         * Returning when the last descriptor and the last timer are gone is
         * deliberate: a loop with nothing registered and no deadline would block
         * forever with no thread able to wake it except through @ref stop, and a
         * server that has finished its work should fall out of its loop rather
         * than hang in it. Register something, or keep a timer alive, to hold a
         * loop open through a quiet moment.
         */
        THROWS(ReactorException, PollerException)
        void run() {
            if (_running) {
                throw ReactorException("the loop is already running");
            }

            _running = true;
            _stopping.store(false);
            try {
                while (!_stopping.load()) {
                    drop_cancelled_roots();
                    if (_handles.empty() && _heap.empty()) {
                        break;
                    }
                    run_once();
                }
            } catch (...) {
                _running = false;
                throw;
            }
            _running = false;
        }

        /**
         * @brief Ends a @ref run from any thread.
         * @throws PollerException if the wake-up descriptor cannot be written.
         *
         * The one member safe to call from a thread other than the one in
         * @ref run. The loop finishes the turn it is in - handlers already
         * dispatched are not interrupted - and then returns.
         *
         * Calling it while no loop is running still takes effect: the next
         * @ref run clears the flag as it starts, so a stop cannot leak into a
         * later loop and end it before its first wait.
         */
        THROWS(PollerException)
        void stop() {
            _stopping.store(true);
            _poller.wake();
        }

        /**
         * @brief Whether @ref run is on the stack.
         * @return true if the loop is running, false otherwise.
         */
        [[nodiscard]]
        bool is_running() const noexcept {
            return _running;
        }

        /**
         * @brief The Poller underneath, for the operations the Reactor does not wrap.
         * @return A reference to the owned Poller.
         *
         * Registration must go through the Reactor, which is why this is not a
         * way to call Poller::add: a descriptor registered behind the Reactor's
         * back has a token it did not issue and a handler it does not have.
         */
        [[nodiscard]]
        constexpr const Poller& poller() const noexcept {
            return _poller;
        }
    };
}
