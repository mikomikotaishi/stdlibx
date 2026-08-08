#pragma once

/**
 * @file darwin.constants.inl
 * @brief Numeric values of the Darwin-only macros.
 */
export namespace stdx::os::darwin {
    /**
     * @namespace sys
     * @brief Darwin system operations.
     */
    namespace sys {
        /**
         * <sys/event.h> constants
         *
         * kqueue is not epoll with different names. epoll takes one event mask
         * per descriptor; kqueue takes one *filter* per direction, so a
         * descriptor watched for both read and write is two registrations. The
         * filter numbers are negative, and end-of-stream arrives as EV_EOF in
         * the flags rather than as a distinct EPOLLRDHUP bit.
         */

        inline constexpr i16 EVFILT_READ = -1; ///< Readable, or the peer has shut down its writing half.
        inline constexpr i16 EVFILT_WRITE = -2; ///< Writable.
        inline constexpr i16 EVFILT_AIO = -3; ///< Attached to aio requests.
        inline constexpr i16 EVFILT_VNODE = -4; ///< Attached to vnodes.
        inline constexpr i16 EVFILT_PROC = -5; ///< Attached to a process.
        inline constexpr i16 EVFILT_SIGNAL = -6; ///< Signal delivery.
        inline constexpr i16 EVFILT_TIMER = -7; ///< Timers.
        inline constexpr i16 EVFILT_MACHPORT = -8; ///< Mach port sets.
        inline constexpr i16 EVFILT_FS = -9; ///< Filesystem events.
        inline constexpr i16 EVFILT_USER = -10; ///< User-triggered events; how a kqueue is woken by hand.
        inline constexpr i16 EVFILT_VM = -12; ///< Virtual memory events.
        inline constexpr i16 EVFILT_EXCEPT = -15; ///< Exceptional conditions, out-of-band data included.

        inline constexpr u16 EV_ADD = 0x0001; ///< Add the event to the queue, and enable it.
        inline constexpr u16 EV_DELETE = 0x0002; ///< Remove the event from the queue.
        inline constexpr u16 EV_ENABLE = 0x0004; ///< Enable an event that was disabled.
        inline constexpr u16 EV_DISABLE = 0x0008; ///< Stop reporting the event without removing it.
        inline constexpr u16 EV_ONESHOT = 0x0010; ///< Report once, then delete.
        inline constexpr u16 EV_CLEAR = 0x0020; ///< Edge-triggered: clear the state once reported.
        inline constexpr u16 EV_RECEIPT = 0x0040; ///< Report the outcome of the change itself.
        inline constexpr u16 EV_DISPATCH = 0x0080; ///< Disable immediately after reporting.
        inline constexpr u16 EV_UDATA_SPECIFIC = 0x0100; ///< Treat udata as part of the event's identity.
        inline constexpr u16 EV_VANISHED = 0x0200; ///< The thing being watched has gone away.
        inline constexpr u16 EV_FLAG0 = 0x1000; ///< Filter-specific.
        inline constexpr u16 EV_FLAG1 = 0x2000; ///< Filter-specific.
        inline constexpr u16 EV_SYSFLAGS = 0xF000; ///< Reserved by the kernel.
        inline constexpr u16 EV_ERROR = 0x4000; ///< The change failed; data carries the errno.
        inline constexpr u16 EV_EOF = 0x8000; ///< End of stream; kqueue's answer to EPOLLRDHUP.

        /**
         * @brief Fires an EVFILT_USER event that was registered earlier.
         *
         * An fflags value rather than a flags one, and the reason a kqueue needs
         * no self-pipe to be woken by hand: Darwin has no eventfd, but EVFILT_USER
         * is a queue entry the caller triggers directly.
         */
        inline constexpr u32 NOTE_TRIGGER = 0x01000000;

        /**
         * <sys/socket.h> constants, Apple extensions
         */

        /**
         * @brief Suppresses SIGPIPE for this socket's whole lifetime.
         *
         * Set once per descriptor rather than per send, and **not inherited by an
         * accepted socket**, so a listener having it says nothing about its
         * peers. The alternative is POSIX's MSG_NOSIGNAL, which Darwin also has;
         * this is the belt-and-braces option that predates it.
         */
        inline constexpr i32 SO_NOSIGPIPE = 0x1022;

        inline constexpr i32 SO_NREAD = 0x1020; ///< Bytes available on the first packet.
    }

    /**
     * @namespace netinet
     * @brief Darwin internet protocol operations.
     */
    namespace netinet {
        /**
         * <netinet/tcp.h> constants
         *
         * Darwin spells the idle-time knob TCP_KEEPALIVE where Linux spells it
         * TCP_KEEPIDLE, and the two are not aliases of one another - each
         * platform defines only its own. The interval and count options do share
         * their names.
         */

        inline constexpr i32 TCP_KEEPALIVE = 0x10; ///< Idle seconds before the first keepalive probe.
        inline constexpr i32 TCP_KEEPINTVL = 0x101; ///< Seconds between probes.
        inline constexpr i32 TCP_KEEPCNT = 0x102; ///< Unanswered probes before the connection is dropped.
    }

    #ifdef __APPLE__
    static_assert(sys::EVFILT_READ == captured::EVFILT_READ_VALUE);
    static_assert(sys::EVFILT_WRITE == captured::EVFILT_WRITE_VALUE);
    static_assert(sys::EVFILT_USER == captured::EVFILT_USER_VALUE);
    static_assert(sys::EVFILT_TIMER == captured::EVFILT_TIMER_VALUE);
    static_assert(sys::EVFILT_EXCEPT == captured::EVFILT_EXCEPT_VALUE);

    static_assert(sys::EV_ADD == captured::EV_ADD_VALUE);
    static_assert(sys::EV_DELETE == captured::EV_DELETE_VALUE);
    static_assert(sys::EV_ENABLE == captured::EV_ENABLE_VALUE);
    static_assert(sys::EV_DISABLE == captured::EV_DISABLE_VALUE);
    static_assert(sys::EV_ONESHOT == captured::EV_ONESHOT_VALUE);
    static_assert(sys::EV_CLEAR == captured::EV_CLEAR_VALUE);
    static_assert(sys::EV_RECEIPT == captured::EV_RECEIPT_VALUE);
    static_assert(sys::EV_DISPATCH == captured::EV_DISPATCH_VALUE);
    static_assert(sys::EV_ERROR == captured::EV_ERROR_VALUE);
    static_assert(sys::EV_EOF == captured::EV_EOF_VALUE);

    static_assert(sys::NOTE_TRIGGER == captured::NOTE_TRIGGER_VALUE);

    static_assert(sys::SO_NOSIGPIPE == captured::SO_NOSIGPIPE_VALUE);
    static_assert(sys::SO_NREAD == captured::SO_NREAD_VALUE);

    static_assert(netinet::TCP_KEEPALIVE == captured::TCP_KEEPALIVE_VALUE);
    static_assert(netinet::TCP_KEEPINTVL == captured::TCP_KEEPINTVL_VALUE);
    static_assert(netinet::TCP_KEEPCNT == captured::TCP_KEEPCNT_VALUE);
    #endif
}
