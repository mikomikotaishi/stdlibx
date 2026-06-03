#pragma once

/**
 * @namespace stdx::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace stdx::sys {
    using Handler = void (*)(int);    
    
    using SignalAtomic = std::sig_atomic_t;

    using std::signal;
    using std::raise;

    /**
     * @class Signal
     * @brief Represents a system signal that can be raised and handled.
     */
    class Signal {
    private:
        int sig;
    public:
        using Handler = void (*)(int);
        
        // Signal handler constants
        static inline const Handler DEFAULT = SIG_DFL; ///< Default signal handler.
        static inline const Handler IGNORE = SIG_IGN; ///< Ignore signal handler.
        static inline const Handler ERROR = SIG_ERR; ///< Error return value.

        // Signal number constants
        static constexpr int HANGUP = SIGHUP; ///< Hangup.
        static constexpr int INTERRUPT = SIGINT; ///< Interactive attention signal.
        static constexpr int QUIT = SIGQUIT; ///< Quit.
        static constexpr int ILLEGAL = SIGILL; ///< Illegal instruction.
        static constexpr int TRAP = SIGTRAP; ///< Trace/breakpoint trap.
        static constexpr int ABORT = SIGABRT; ///< Abnormal termination.
        static constexpr int BUS_ERROR = SIGBUS; ///< Bus error.
        static constexpr int FLOATING_POINT_EXCEPTION = SIGFPE; ///< Erroneous arithmetic operation.
        static constexpr int KILL = SIGKILL; ///< Killed.
        static constexpr int USER_DEFINED_1 = SIGUSR1; ///< User-defined signal 1.
        static constexpr int SEGMENTATION_VIOLATION = SIGSEGV; ///< Invalid access to storage.
        static constexpr int USER_DEFINED_2 = SIGUSR2; ///< User-defined signal 2.
        static constexpr int BROKEN_PIPE = SIGPIPE; ///< Broken pipe.
        static constexpr int ALARM = SIGALRM; ///< Alarm clock.
        static constexpr int TERMINATE = SIGTERM; ///< Termination request.
        static constexpr int STACK_FAULT = SIGSTKFLT; ///< Stack fault (obsolete).
        static constexpr int CHILD = SIGCHLD; ///< Child terminated or stopped.
        static constexpr int CONTINUE = SIGCONT; ///< Continue.
        static constexpr int STOP = SIGSTOP; ///< Stop, unblockable.
        static constexpr int TERMINAL_STOP = SIGTSTP; ///< Keyboard stop.
        static constexpr int TERMINAL_INPUT = SIGTTIN; ///< Background read from control terminal.
        static constexpr int TERMINAL_OUTPUT = SIGTTOU; ///< Background write to control terminal.
        static constexpr int URGENT = SIGURG; ///< Urgent data is available at a socket.
        static constexpr int CPU_LIMIT = SIGXCPU; ///< CPU time limit exceeded.
        static constexpr int FILE_SIZE_LIMIT = SIGXFSZ; ///< File size limit exceeded.
        static constexpr int VIRTUAL_ALARM = SIGVTALRM; ///< Virtual timer expired.
        static constexpr int PROFILING = SIGPROF; ///< Profiling timer expired.
        static constexpr int WINDOW_CHANGE = SIGWINCH; ///< Window size change (4.3 BSD, Sun).
        static constexpr int POLL = SIGPOLL; ///< Pollable event occurred (System V).
        static constexpr int POWER_FAILURE = SIGPWR; ///< Power failure imminent.
        static constexpr int BAD_SYSTEM_CALL = SIGSYS; ///< Bad system call.
        static constexpr int IO = SIGIO; ///< I/O now possible (4.2 BSD).
        static constexpr int IOT = SIGIOT; ///< IOT instruction, abort() on a PDP-11.

        /**
         * @brief Constructs a Signal object for the specified signal number.
         * @param sig The signal number.
         */
        explicit Signal(int sig) noexcept:
            sig{sig} {}

        /**
         * @brief Gets the signal number.
         * @return The signal number.
         */
        [[nodiscard]]
        int number() const noexcept {
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
        int raise() const noexcept {
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
        static Handler handle(int sig, Handler handler) noexcept {
            return std::signal(sig, handler);
        }

        /**
         * @brief Raises the specified signal.
         * @param sig The signal number.
         * @return 0 on success, non-zero on failure.
         */
        static int raise(int sig) noexcept {
            return std::raise(sig);
        }
    };
}
