#pragma once

/**
 * @namespace stdx::sys
 * @brief Standard library system operations.
 */
export namespace stdx::sys {
    using std::signal;
    using std::raise;

    /**
     * @class Signal
     * @brief Represents a system signal that can be raised and handled.
     */
    class Signal {
    public:
        using Handler = void (*)(i32);
        using AtomicStatus = std::sig_atomic_t;
        
        // Signal handler constants
        static inline const Handler DEFAULT = SIG_DFL; ///< Default signal handler.
        static inline const Handler IGNORE = SIG_IGN; ///< Ignore signal handler.
        static inline const Handler ERROR = SIG_ERR; ///< Error return value.

        // Signal number constants
        static constexpr i32 HANGUP = SIGHUP; ///< Hangup.
        static constexpr i32 INTERRUPT = SIGINT; ///< Interactive attention signal.
        static constexpr i32 QUIT = SIGQUIT; ///< Quit.
        static constexpr i32 ILLEGAL = SIGILL; ///< Illegal instruction.
        static constexpr i32 TRAP = SIGTRAP; ///< Trace/breakpoint trap.
        static constexpr i32 ABORT = SIGABRT; ///< Abnormal termination.
        static constexpr i32 BUS_ERROR = SIGBUS; ///< Bus error.
        static constexpr i32 FLOATING_POINT_EXCEPTION = SIGFPE; ///< Erroneous arithmetic operation.
        static constexpr i32 KILL = SIGKILL; ///< Killed.
        static constexpr i32 USER_DEFINED_1 = SIGUSR1; ///< User-defined signal 1.
        static constexpr i32 SEGMENTATION_VIOLATION = SIGSEGV; ///< Invalid access to storage.
        static constexpr i32 USER_DEFINED_2 = SIGUSR2; ///< User-defined signal 2.
        static constexpr i32 BROKEN_PIPE = SIGPIPE; ///< Broken pipe.
        static constexpr i32 ALARM = SIGALRM; ///< Alarm clock.
        static constexpr i32 TERMINATE = SIGTERM; ///< Termination request.
        static constexpr i32 STACK_FAULT = SIGSTKFLT; ///< Stack fault (obsolete).
        static constexpr i32 CHILD = SIGCHLD; ///< Process terminated or stopped.
        static constexpr i32 CONTINUE = SIGCONT; ///< Continue.
        static constexpr i32 STOP = SIGSTOP; ///< Stop, unblockable.
        static constexpr i32 TERMINAL_STOP = SIGTSTP; ///< Keyboard stop.
        static constexpr i32 TERMINAL_INPUT = SIGTTIN; ///< Background read from control terminal.
        static constexpr i32 TERMINAL_OUTPUT = SIGTTOU; ///< Background write to control terminal.
        static constexpr i32 URGENT = SIGURG; ///< Urgent data is available at a socket.
        static constexpr i32 CPU_LIMIT = SIGXCPU; ///< CPU time limit exceeded.
        static constexpr i32 FILE_SIZE_LIMIT = SIGXFSZ; ///< File size limit exceeded.
        static constexpr i32 VIRTUAL_ALARM = SIGVTALRM; ///< Virtual timer expired.
        static constexpr i32 PROFILING = SIGPROF; ///< Profiling timer expired.
        static constexpr i32 WINDOW_CHANGE = SIGWINCH; ///< Window size change (4.3 BSD, Sun).
        static constexpr i32 POLL = SIGPOLL; ///< Pollable event occurred (System V).
        static constexpr i32 POWER_FAILURE = SIGPWR; ///< Power failure imminent.
        static constexpr i32 BAD_SYSTEM_CALL = SIGSYS; ///< Bad system call.
        static constexpr i32 IO = SIGIO; ///< I/O now possible (4.2 BSD).
        static constexpr i32 IOT = SIGIOT; ///< IOT instruction, abort() on a PDP-11.
    private:
        i32 sig;
    public:
        /**
         * @brief Constructs a Signal object for the specified signal number.
         * @param sig The signal number.
         */
        explicit Signal(i32 sig) noexcept:
            sig{sig} {}

        /**
         * @brief Gets the signal number.
         * @return The signal number.
         */
        [[nodiscard]]
        i32 number() const noexcept {
            return sig;
        }

        /**
         * @brief Sets a signal handler for this signal.
         * @param handler The signal handler function.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        Handler set_handler(Handler handler) const noexcept {
            return std::signal(sig, handler);
        }

        /**
         * @brief Raises this signal.
         * @return 0 on success, non-zero on failure.
         */
        i32 raise() const noexcept {
            return std::raise(sig);
        }

        /**
         * @brief Ignores this signal.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        Handler ignore() const noexcept {
            return std::signal(sig, IGNORE);
        }

        /**
         * @brief Resets this signal to default behavior.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        Handler reset() const noexcept {
            return std::signal(sig, DEFAULT);
        }

        // Static methods for direct access
        /**
         * @brief Sets a signal handler for the specified signal.
         * @param sig The signal number.
         * @param handler The signal handler function.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        static Handler handle(i32 sig, Handler handler) noexcept {
            return std::signal(sig, handler);
        }

        /**
         * @brief Raises the specified signal.
         * @param sig The signal number.
         * @return 0 on success, non-zero on failure.
         */
        static i32 raise(i32 sig) noexcept {
            return std::raise(sig);
        }
    };
}
