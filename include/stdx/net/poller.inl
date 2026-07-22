#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class PollerException
     * @brief Exception class for handling readiness-notification errors.
     * @extends IOException
     *
     * The PollerException class is used to represent an exception that occurs when the
     * operating system's readiness mechanism fails, as opposed to a failure on any one
     * of the descriptors registered with it.
     */
    class PollerException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @enum Interest
     * @brief The readiness a descriptor is registered to be told about.
     *
     * Errors and hang-ups are always reported and are deliberately absent here:
     * every backend delivers them whether or not they were asked for, so an
     * interest in them would be a promise the interface cannot keep either way.
     */
    enum class Interest: u8 {
        READ = 1, ///< Report when the descriptor has data, a pending connection, or end-of-stream.
        WRITE = 2, ///< Report when the descriptor can accept bytes, or a connect has settled.
        READ_WRITE = 3, ///< Report both.
    };

    /**
     * @struct Event
     * @brief One descriptor's readiness, as reported by a completed Poller::wait.
     *
     * A single event may carry several of these at once - a peer that sends and
     * then closes leaves a descriptor both @ref readable and @ref read_hangup -
     * so they are flags to be tested, not a state to be switched on.
     */
    struct Event {
        u64 token = 0; ///< The value handed to Poller::add for this descriptor.
        bool readable = false; ///< Reading will not block; may mean end-of-stream rather than data.
        bool writable = false; ///< Writing will not block, or a pending connect has settled.
        bool error = false; ///< The descriptor has a pending error; read it with Socket::take_error.
        bool hangup = false; ///< The connection is down in both directions. Reported even without an Interest in it.
        bool read_hangup = false; ///< The peer shut down its writing half; reads will drain, then report end-of-stream. Always false on Windows, which does not distinguish it.
    };
}

/**
 * @namespace stdx::net
 * @brief Platform glue for readiness notification.
 */
namespace stdx::net {
    /**
     * @internal
     * @brief The wait timeout, in the milliseconds both backends take.
     * @param timeout How long to block, or an empty Optional to block indefinitely.
     * @return -1 to block indefinitely, otherwise a non-negative count of milliseconds.
     *
     * epoll_wait and WSAPoll both take an int, so a duration past its range is
     * clamped rather than allowed to wrap into a short wait or an accidental -1.
     */
    [[nodiscard]]
    inline i32 wait_timeout(const Optional<Milliseconds>& timeout) noexcept {
        static constexpr i64 CEILING = 0x7fffffff;
        if (!timeout.has_value()) {
            return -1;
        }
        const i64 count = timeout->count();
        if (count <= 0) {
            return 0;
        }
        return static_cast<i32>(count < CEILING ? count : CEILING);
    }

    #ifdef _WIN32
    /**
     * @internal
     * @brief The WSAPoll flags for @p interest.
     * @param interest The readiness to register for.
     * @return The mask to put in a WSAPOLLFD's events field.
     *
     * POLLRDBAND is left out of the read mask on purpose: it means out-of-band
     * data, which a stream reader neither expects nor drains, and asking for it
     * would wake the reactor for bytes no read is going to consume. POLLERR,
     * POLLHUP and POLLNVAL are rejected outright by WSAPoll if requested, and
     * arrive in revents regardless.
     */
    [[nodiscard]]
    inline i16 poll_flags(Interest interest) noexcept {
        switch (interest) {
            case Interest::READ:
                return win32::POLLRDNORM;
            case Interest::WRITE:
                return win32::POLLWRNORM;
            case Interest::READ_WRITE:
                return static_cast<i16>(win32::POLLRDNORM | win32::POLLWRNORM);
        }
        Ops::unreachable();
    }
    #elifdef __linux__
    /**
     * @internal
     * @brief The epoll flags standing for an Interest in reading.
     *
     * RD_HUP rides along with every read registration rather than being an
     * Interest of its own. A peer's half-close is the ordinary way a request
     * ends, and epoll reports it only if asked; a reactor that did not ask sees
     * an endlessly readable descriptor that never yields another byte.
     */
    inline constexpr u32 EPOLL_READ_FLAGS = Ops::to_underlying(linux::sys::EpollEvents::IN) | Ops::to_underlying(linux::sys::EpollEvents::RD_HUP);

    /**
     * @internal
     * @brief The epoll flags standing for an Interest in writing.
     */
    inline constexpr u32 EPOLL_WRITE_FLAGS = Ops::to_underlying(linux::sys::EpollEvents::OUT);

    /**
     * @internal
     * @brief The epoll flags for @p interest.
     * @param interest The readiness to register for.
     * @return The mask to put in an epoll_event.
     */
    [[nodiscard]]
    inline u32 epoll_flags(Interest interest) noexcept {
        switch (interest) {
            case Interest::READ:
                return EPOLL_READ_FLAGS;
            case Interest::WRITE:
                return EPOLL_WRITE_FLAGS;
            case Interest::READ_WRITE:
                return EPOLL_READ_FLAGS | EPOLL_WRITE_FLAGS;
        }
        Ops::unreachable();
    }
    #endif

    /**
     * @internal
     * @brief Throws the exception describing a failed readiness call.
     * @param error The platform error code.
     * @param operation The name of the call that failed, for the message.
     *
     * Defined out-of-line so the thrown type is emitted once, inside the module,
     * rather than in every consumer that inlines a poller operation.
     */
    [[noreturn]]
    void raise_poller_error(i32 error, StringView operation) {
        throw PollerException(Ops::fmt("{} failed: {}", operation, describe_socket_error(error)));
    }
}

export namespace stdx::net {
    #if defined(_WIN32) || defined(__linux__)
    /**
     * @class Poller
     * @brief Waits on many descriptors at once and reports which are ready.
     *
     * A Poller turns the one-thread-per-connection shape into one thread for
     * any number of connections: register the descriptors, block in @ref wait,
     * and drive whichever come back ready. It pairs with the `try_` half of
     * Socket's interface - put a socket in non-blocking mode, hand
     * Socket::native_handle to @ref add, and treat an empty Optional from a
     * `try_` call as "wait for me again" rather than as a failure.
     *
     * Registration is by descriptor, not by Socket: a Poller never owns, closes
     * or outlives the sockets it watches. **Remove a descriptor before closing
     * it.** On Linux, closing removes the registration only when the last
     * descriptor referring to the open file description goes away, so a
     * `dup`-ed socket that is closed keeps firing events under the old token.
     *
     * Readiness is level-triggered. A descriptor that stays readable is
     * reported by every @ref wait until it is drained, which is the forgiving
     * behavior: the edge-triggered alternative reports the transition once and
     * silently stops if a caller ever fails to read to `EWOULDBLOCK`.
     *
     * The class is not thread-safe, with one exception: @ref wake may be called
     * from any thread, and is the supported way to break another thread out of
     * a @ref wait so it can notice that it should stop.
     */
    class [[nodiscard]] Poller {
    public:
        /**
         * @brief The platform's descriptor type, matching the one Socket hands out.
         */
        using NativeHandle = Socket::NativeHandle;

        /**
         * @brief The token reserved for the internal @ref wake descriptor.
         *
         * @ref add and @ref modify reject it, so a caller cannot silently take
         * over the wake-up path by choosing the same value.
         */
        static constexpr u64 WAKE_TOKEN = ~static_cast<u64>(0);
    private:
        #ifdef _WIN32
        using NativeEvent = win32::WinPollFd; ///< The array element WSAPoll reads and writes.

        /**
         * @internal
         * @struct Registration
         * @brief One caller registration.
         *
         * WSAPoll keeps no state of its own, so the poller keeps the table the
         * kernel would otherwise hold and rebuilds the descriptor array from it
         * on every wait.
         */
        struct Registration {
            NativeHandle handle; ///< The watched descriptor.
            Interest interest; ///< The readiness it was registered for.
            u64 token; ///< The value to report it under.
        };

        Vector<Registration> _registrations; ///< Every descriptor the caller has added, in registration order.
        Socket _wake; ///< A loopback datagram socket that @ref wake sends to, standing in for the eventfd Windows lacks.
        Endpoint _wake_endpoint; ///< The wake socket's own address, which is where @ref wake sends.
        #else
        using NativeEvent = linux::sys::EpollEvent; ///< The array element epoll_wait writes.

        static constexpr usize INITIAL_CAPACITY = 16; ///< How many events a single wait can report before the buffer grows.
        static constexpr usize MAXIMUM_CAPACITY = 1024; ///< The ceiling on that growth; a wait reporting this many simply reports the rest next time.

        i32 _epoll = -1; ///< The epoll instance, or -1 once moved from.
        i32 _wake = -1; ///< The eventfd @ref wake writes to, or -1 once moved from.
        usize _registered = 0; ///< How many descriptors the caller has added.
        #endif

        Vector<NativeEvent> _native; ///< The buffer handed to the operating system, reused across waits.
        Vector<Event> _events; ///< The translated events @ref wait returns a view of.

        /**
         * @internal
         * @brief Rejects the reserved token.
         * @param token The caller's token.
         * @throws PollerException if @p token is WAKE_TOKEN.
         */
        THROWS(PollerException)
        static void reject_reserved(u64 token) {
            if (token == WAKE_TOKEN) {
                throw PollerException("Poller::WAKE_TOKEN is reserved for the internal wake-up descriptor");
            }
        }

        #ifdef _WIN32
        /**
         * @internal
         * @brief The registration for @p handle.
         * @param handle The descriptor to look for.
         * @return A pointer to the entry, or nullptr if it is not registered.
         *
         * A linear scan, which matches the backend: WSAPoll already walks the
         * whole set on every wait, so an index would not change the cost class.
         */
        [[nodiscard]]
        Registration* find_registration(NativeHandle handle) noexcept {
            for (Registration& entry: _registrations) {
                if (entry.handle == handle) {
                    return &entry;
                }
            }
            return nullptr;
        }

        /**
         * @internal
         * @brief Opens the wake-up socket.
         * @throws PollerException if it cannot be opened.
         *
         * Windows has no eventfd and no pipe that select-style calls accept, so
         * the portable stand-in is a datagram socket bound to loopback that
         * sends to its own address.
         */
        THROWS(PollerException)
        void open_backend() {
            try {
                _wake = Socket(IPAddress::Family::IPV4, Socket::Type::DATAGRAM);
                _wake.bind(Endpoint(IPAddress::loopback(IPAddress::Family::IPV4), 0));
                _wake.set_blocking(false);
                _wake_endpoint = _wake.local_endpoint();
            } catch (const SocketException& cause) {
                throw PollerException(
                    Ops::fmt("the poller's wake-up socket could not be opened: {}", cause.what())
                );
            }
        }

        /**
         * @internal
         * @brief Releases the backend.
         *
         * Nothing to do: the wake socket closes itself, and the registration
         * table is plain memory.
         */
        void close_backend() noexcept {
        }

        /**
         * @internal
         * @brief Adds @p handle to the registration table.
         * @param handle The descriptor to watch.
         * @param interest The readiness to report on.
         * @param token The value to report it under.
         * @throws PollerException if @p handle is already registered.
         */
        THROWS(PollerException)
        void register_handle(NativeHandle handle, Interest interest, u64 token) {
            if (find_registration(handle) != nullptr) {
                throw PollerException("the descriptor is already registered with this poller");
            }
            _registrations.push_back(Registration{handle, interest, token});
        }

        /**
         * @internal
         * @brief Replaces @p handle's interest and token.
         * @param handle A registered descriptor.
         * @param interest The readiness to report on from now on.
         * @param token The value to report it under from now on.
         * @throws PollerException if @p handle is not registered.
         */
        THROWS(PollerException)
        void reregister_handle(NativeHandle handle, Interest interest, u64 token) {
            Registration* const entry = find_registration(handle);
            if (entry == nullptr) {
                throw PollerException("the descriptor is not registered with this poller");
            }
            entry->interest = interest;
            entry->token = token;
        }

        /**
         * @internal
         * @brief Drops @p handle from the registration table.
         * @param handle A registered descriptor.
         * @throws PollerException if @p handle is not registered.
         */
        THROWS(PollerException)
        void deregister_handle(NativeHandle handle) {
            for (usize index = 0; index < _registrations.size(); ++index) {
                if (_registrations[index].handle == handle) {
                    _registrations.erase(_registrations.begin() + static_cast<isize>(index));
                    return;
                }
            }
            throw PollerException("the descriptor is not registered with this poller");
        }

        /**
         * @internal
         * @brief Sends the wake-up datagram.
         */
        THROWS(PollerException)
        void signal_wake() {
            static constexpr Array<byte, 1> PAYLOAD{};
            try {
                // An empty Optional means the send buffer is full, which means a
                // datagram is already queued - the state the caller wanted.
                (void)_wake.try_send_to(Span<const byte>(PAYLOAD), _wake_endpoint);
            } catch (const SocketException& cause) {
                throw PollerException(Ops::fmt("the poller could not be woken: {}", cause.what()));
            }
        }

        /**
         * @internal
         * @brief Consumes every queued wake-up datagram.
         *
         * The socket is non-blocking, so the receive that finds nothing reports
         * an empty Optional and ends the loop. Draining collapses any number of
         * concurrent @ref wake calls into the single wakeup they asked for.
         */
        void drain_wake() noexcept {
            Array<byte, 64> scratch;
            try {
                while (_wake.try_receive_from(Span<byte>(scratch)).has_value()) {
                }
            } catch (const SocketException& _) {
                // Nothing useful to do while draining a wake-up; the next wait
                // will find the socket in whatever state it is really in.
            }
        }

        /**
         * @internal
         * @brief Polls every registered descriptor and fills @ref _events.
         * @param milliseconds How long to block, or -1 to block indefinitely.
         * @throws PollerException if the poll itself fails.
         *
         * The wake socket is appended last so its slot is known without a
         * search, and so an empty registration table still gives WSAPoll the
         * one descriptor it insists on being given.
         */
        THROWS(PollerException)
        void collect(i32 milliseconds) {
            _native.clear();
            _native.reserve(_registrations.size() + 1);
            for (const Registration& entry: _registrations) {
                _native.push_back(NativeEvent{
                    .fd = entry.handle,
                    .events = poll_flags(entry.interest),
                    .revents = 0
                });
            }
            _native.push_back(NativeEvent{
                .fd = _wake.native_handle(),
                .events = win32::POLLRDNORM,
                .revents = 0
            });

            const i32 count = win32::WSAPoll(
                _native.data(),
                static_cast<u32>(_native.size()),
                milliseconds
            );
            if (count == win32::SOCKET_ERROR) {
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    return;
                }
                raise_poller_error(error, "WSAPoll");
            }
            if (count == 0) {
                return;
            }

            if ((_native.back().revents & win32::POLLRDNORM) != 0) {
                drain_wake();
            }
            for (usize index = 0; index < _registrations.size(); ++index) {
                const i16 flags = _native[index].revents;
                if (flags == 0) {
                    continue;
                }
                _events.push_back(Event {
                    .token = _registrations[index].token,
                    .readable = (flags & (win32::POLLRDNORM | win32::POLLRDBAND)) != 0,
                    .writable = (flags & win32::POLLWRNORM) != 0,
                    .error = (flags & (win32::POLLERR | win32::POLLNVAL)) != 0,
                    .hangup = (flags & win32::POLLHUP) != 0,
                    .read_hangup = false
                });
            }
        }
        #else
        /**
         * @internal
         * @brief Creates the epoll instance and its wake-up descriptor.
         * @throws PollerException if either cannot be created.
         */
        THROWS(PollerException)
        void open_backend() {
            _native.resize(INITIAL_CAPACITY);
            _epoll = linux::sys::epoll_create1(linux::sys::EPOLL_CLOEXEC);
            if (_epoll < 0) {
                raise_poller_error(unix::errnov(), "epoll_create1");
            }
            _wake = linux::sys::eventfd(0, linux::sys::EFD_CLOEXEC | linux::sys::EFD_NONBLOCK);
            if (_wake < 0) {
                const i32 error = unix::errnov();
                close_backend();
                raise_poller_error(error, "eventfd");
            }
            linux::sys::EpollEvent registration{};
            registration.events = EPOLL_READ_FLAGS;
            registration.data.u64 = WAKE_TOKEN;
            const i32 added = linux::sys::epoll_ctl(
                _epoll,
                static_cast<i32>(linux::sys::EpollControl::ADD),
                _wake,
                &registration
            );
            if (added < 0) {
                const i32 error = unix::errnov();
                close_backend();
                raise_poller_error(error, "epoll_ctl(ADD, eventfd)");
            }
        }

        /**
         * @internal
         * @brief Closes both owned descriptors and marks them absent.
         */
        void close_backend() noexcept {
            if (_wake >= 0) {
                unix::close(_wake);
                _wake = -1;
            }
            if (_epoll >= 0) {
                unix::close(_epoll);
                _epoll = -1;
            }
        }

        /**
         * @internal
         * @brief Applies one epoll_ctl operation.
         * @param operation ADD, MOD or DEL.
         * @param handle The descriptor to apply it to.
         * @param interest The readiness to register, ignored by DEL.
         * @param token The value to report back, ignored by DEL.
         * @param what The name of the operation, for the message.
         * @throws PollerException if the kernel rejects it.
         *
         * Applied immediately rather than buffered into a change list. The
         * syscall per change is the price of reporting a bad descriptor at the
         * call that supplied it instead of at the next wait, where it would
         * arrive with nothing to attribute it to.
         */
        THROWS(PollerException)
        void control(
            linux::sys::EpollControl operation,
            NativeHandle handle,
            Interest interest,
            u64 token,
            StringView what
        ) {
            linux::sys::EpollEvent registration{};
            registration.events = epoll_flags(interest);
            registration.data.u64 = token;
            if (linux::sys::epoll_ctl(_epoll, static_cast<i32>(operation), handle, &registration) < 0) {
                raise_poller_error(unix::errnov(), what);
            }
        }

        /**
         * @internal
         * @brief Registers @p handle with the epoll instance.
         * @param handle The descriptor to watch.
         * @param interest The readiness to report on.
         * @param token The value to report it under.
         * @throws PollerException if the kernel rejects it.
         */
        THROWS(PollerException)
        void register_handle(NativeHandle handle, Interest interest, u64 token) {
            control(linux::sys::EpollControl::ADD, handle, interest, token, "epoll_ctl(ADD)");
            ++_registered;
        }

        /**
         * @internal
         * @brief Replaces @p handle's interest and token.
         * @param handle A registered descriptor.
         * @param interest The readiness to report on from now on.
         * @param token The value to report it under from now on.
         * @throws PollerException if the kernel rejects it.
         */
        THROWS(PollerException)
        void reregister_handle(NativeHandle handle, Interest interest, u64 token) {
            control(linux::sys::EpollControl::MOD, handle, interest, token, "epoll_ctl(MOD)");
        }

        /**
         * @internal
         * @brief Drops @p handle from the epoll instance.
         * @param handle A registered descriptor.
         * @throws PollerException if the kernel rejects it.
         */
        THROWS(PollerException)
        void deregister_handle(NativeHandle handle) {
            const i32 removed = linux::sys::epoll_ctl(
                _epoll,
                static_cast<i32>(linux::sys::EpollControl::DEL),
                handle,
                nullptr
            );
            if (removed < 0) {
                raise_poller_error(unix::errnov(), "epoll_ctl(DEL)");
            }
            if (_registered > 0) {
                --_registered;
            }
        }

        /**
         * @internal
         * @brief Raises the wake-up descriptor's count.
         */
        THROWS(PollerException)
        void signal_wake() {
            if (linux::sys::eventfd_write(_wake, 1) < 0) {
                const i32 error = unix::errnov();
                if (!is_would_block(error)) {
                    raise_poller_error(error, "eventfd_write");
                }
            }
        }

        /**
         * @internal
         * @brief Clears the wake-up descriptor's pending count.
         *
         * The eventfd is non-blocking, so the second read fails with EAGAIN and
         * ends the loop. Draining collapses any number of concurrent @ref wake
         * calls into the single wakeup they asked for.
         */
        void drain_wake() noexcept {
            linux::sys::EventFdValue value = 0;
            while (linux::sys::eventfd_read(_wake, &value) == 0) {
            }
        }

        /**
         * @internal
         * @brief Enlarges the output buffer when a wait filled it.
         * @param count How many events the last wait reported.
         *
         * A saturated buffer means there were probably more ready descriptors
         * than room to report them. They are not lost - level-triggered
         * readiness redelivers them on the next wait - but a buffer that keeps
         * filling turns one wait into several.
         */
        void grow_if_saturated(i32 count) {
            const usize reported = static_cast<usize>(count);
            if (reported < _native.size() || _native.size() >= MAXIMUM_CAPACITY) {
                return;
            }
            const usize doubled = _native.size() * 2;
            _native.resize(doubled < MAXIMUM_CAPACITY ? doubled : MAXIMUM_CAPACITY);
        }

        /**
         * @internal
         * @brief Waits on the epoll instance and fills @ref _events.
         * @param milliseconds How long to block, or -1 to block indefinitely.
         * @throws PollerException if the wait itself fails.
         */
        THROWS(PollerException)
        void collect(i32 milliseconds) {
            const i32 count = linux::sys::epoll_wait(
                _epoll,
                _native.data(),
                static_cast<i32>(_native.size()),
                milliseconds
            );
            if (count < 0) {
                const i32 error = unix::errnov();
                if (is_interrupted(error)) {
                    return;
                }
                raise_poller_error(error, "epoll_wait");
            }
            for (usize index = 0; index < static_cast<usize>(count); ++index) {
                const linux::sys::EpollEvent& raw = _native[index];
                if (raw.data.u64 == WAKE_TOKEN) {
                    drain_wake();
                    continue;
                }
                const u32 flags = raw.events;
                _events.push_back(Event {
                    .token = raw.data.u64,
                    .readable = (flags & Ops::to_underlying(linux::sys::EpollEvents::IN)) != 0,
                    .writable = (flags & Ops::to_underlying(linux::sys::EpollEvents::OUT)) != 0,
                    .error = (flags & Ops::to_underlying(linux::sys::EpollEvents::ERR)) != 0,
                    .hangup = (flags & Ops::to_underlying(linux::sys::EpollEvents::HUP)) != 0,
                    .read_hangup = (flags & Ops::to_underlying(linux::sys::EpollEvents::RD_HUP)) != 0
                });
            }
            grow_if_saturated(count);
        }
        #endif
    public:
        /**
         * @brief Creates an empty poller.
         * @throws PollerException if the platform's readiness mechanism cannot be set up.
         */
        THROWS(PollerException)
        Poller() {
            open_backend();
        }

        Poller(const Poller&) = delete("Poller owns its descriptors and cannot be copied.");

        Poller& operator=(const Poller&) = delete("Poller owns its descriptors and cannot be copied.");

        #ifdef _WIN32
        Poller(Poller&& other) noexcept:
            _registrations{Ops::move(other._registrations)},
            _wake{Ops::move(other._wake)},
            _wake_endpoint{other._wake_endpoint},
            _native{Ops::move(other._native)},
            _events{Ops::move(other._events)} {}

        Poller& operator=(Poller&& other) noexcept {
            if (this != &other) {
                _registrations = Ops::move(other._registrations);
                _wake = Ops::move(other._wake);
                _wake_endpoint = other._wake_endpoint;
                _native = Ops::move(other._native);
                _events = Ops::move(other._events);
            }
            return *this;
        }
        #else
        Poller(Poller&& other) noexcept:
            _epoll{Ops::exchange(other._epoll, -1)},
            _wake{Ops::exchange(other._wake, -1)},
            _registered{Ops::exchange(other._registered, 0)},
            _native{Ops::move(other._native)},
            _events{Ops::move(other._events)} {}

        Poller& operator=(Poller&& other) noexcept {
            if (this != &other) {
                close_backend();
                _epoll = Ops::exchange(other._epoll, -1);
                _wake = Ops::exchange(other._wake, -1);
                _registered = Ops::exchange(other._registered, 0);
                _native = Ops::move(other._native);
                _events = Ops::move(other._events);
            }
            return *this;
        }
        #endif

        ~Poller() {
            close_backend();
        }

        /**
         * @brief Registers @p handle to be reported on.
         * @param handle The descriptor to watch; the Poller does not take ownership of it.
         * @param interest The readiness to be told about.
         * @param token The value to hand back in every Event for this descriptor.
         * @throws PollerException if @p handle is already registered, or @p token is reserved.
         *
         * The token is the caller's own: an index, a small integer id, or a
         * pointer cast to u64. It is the whole of what an Event carries back,
         * so it should be enough to find the connection again without a lookup.
         *
         * On Linux a descriptor that cannot be polled is refused here. On
         * Windows there is nothing to refuse it with, and it surfaces at the
         * next @ref wait as an Event with Event::error set.
         */
        THROWS(PollerException)
        void add(NativeHandle handle, Interest interest, u64 token) {
            reject_reserved(token);
            register_handle(handle, interest, token);
        }

        /**
         * @brief Changes what @p handle is reported on, and under which token.
         * @param handle A descriptor already passed to @ref add.
         * @param interest The readiness to be told about from now on.
         * @param token The value to hand back from now on.
         * @throws PollerException if @p handle is not registered, or @p token is reserved.
         *
         * The usual reason to call this is a write that went short: add WRITE
         * until the rest of the buffer drains, then drop back to READ so an
         * idle connection stops reporting a writability nobody is waiting on.
         */
        THROWS(PollerException)
        void modify(NativeHandle handle, Interest interest, u64 token) {
            reject_reserved(token);
            reregister_handle(handle, interest, token);
        }

        /**
         * @brief Stops reporting on @p handle.
         * @param handle A descriptor already passed to @ref add.
         * @throws PollerException if @p handle is not registered.
         *
         * Call this before closing the descriptor. Afterwards the poller no
         * longer knows it, and removing it again throws.
         */
        THROWS(PollerException)
        void remove(NativeHandle handle) {
            deregister_handle(handle);
        }

        /**
         * @brief Blocks until at least one descriptor is ready, or @p timeout lapses.
         * @param timeout How long to block, or an empty Optional to block until something happens.
         * @return A view of the ready descriptors, empty if the wait timed out or was interrupted.
         * @throws PollerException if the wait itself fails.
         *
         * The returned view belongs to the Poller and stays valid only until the
         * next @ref wait, which reuses the same storage. Copy out of it if the
         * events must outlive the turn of the loop.
         *
         * An empty result is not proof that nothing happened: a signal, or a
         * @ref wake from another thread, both end a wait early and report
         * nothing. Treat waking with no events as ordinary and loop again.
         */
        [[nodiscard]]
        THROWS(PollerException)
        Span<const Event> wait(Optional<Milliseconds> timeout = nullopt) {
            _events.clear();
            collect(wait_timeout(timeout));
            return Span<const Event>(_events.data(), _events.size());
        }

        /**
         * @brief Ends another thread's @ref wait early.
         * @throws PollerException if the wake-up descriptor cannot be written.
         *
         * The one member safe to call from a thread other than the one in
         * @ref wait, and the only way to stop a reactor blocked with no
         * deadline. The waiting thread returns an empty result and no event;
         * what it does next - notice a shutdown flag, drain a work queue - is
         * the caller's business.
         *
         * Repeated calls before the wait returns collapse into one wakeup.
         */
        THROWS(PollerException)
        void wake() {
            signal_wake();
        }

        /**
         * @brief How many descriptors are registered.
         * @return The number of successful @ref add calls, less the @ref remove calls.
         *
         * The internal wake-up descriptor is not counted.
         */
        [[nodiscard]]
        usize registered() const noexcept {
            #ifdef _WIN32
            return _registrations.size();
            #else
            return _registered;
            #endif
        }

        /**
         * @brief Whether this Poller is still usable.
         * @return false only after it has been moved from.
         */
        [[nodiscard]]
        bool is_open() const noexcept {
            #ifdef _WIN32
            return _wake.is_open();
            #else
            return _epoll >= 0;
            #endif
        }

        #ifndef _WIN32
        /**
         * @brief The epoll descriptor itself, for handing to a foreign API.
         * @return The descriptor, or -1 once moved from.
         *
         * An epoll instance is itself pollable, which is how a Poller nests
         * inside somebody else's event loop. Absent on Windows, where WSAPoll
         * has no handle to nest.
         */
        [[nodiscard]]
        constexpr NativeHandle native_handle() const noexcept {
            return _epoll;
        }
        #endif
    };
    #endif
}
