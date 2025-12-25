/**
 * @file csignal.cppm
 * @module core:sys.csignal
 * @brief Module file for standard library C-style signal operations.
 *
 * This file contains the implementation of the C-style signal operations in the standard library.
 */

module;

export module core:sys.csignal;

import :sys.csignal.constants;

using _detail::core::sys::Handler;

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    constexpr int SIGHUP = _detail::core::sys::C_SIGHUP; ///< Hangup.
    constexpr int SIGINT = _detail::core::sys::C_SIGINT; ///< Interactive attention signal.
    constexpr int SIGQUIT = _detail::core::sys::C_SIGQUIT; ///< Quit.
    constexpr int SIGILL = _detail::core::sys::C_SIGILL; ///< Illegal instruction.
    constexpr int SIGTRAP = _detail::core::sys::C_SIGTRAP; ///< Trace/breakpoint trap.
    constexpr int SIGABRT = _detail::core::sys::C_SIGABRT; ///< Abnormal termination.
    constexpr int SIGBUS = _detail::core::sys::C_SIGBUS; ///< Bus error.
    constexpr int SIGFPE = _detail::core::sys::C_SIGFPE; ///< Erroneous arithmetic operation.
    constexpr int SIGKILL = _detail::core::sys::C_SIGKILL; ///< Killed.
    constexpr int SIGUSR1 = _detail::core::sys::C_SIGUSR1; ///< User-defined signal 1.
    constexpr int SIGSEGV = _detail::core::sys::C_SIGSEGV; ///< Invalid access to storage.
    constexpr int SIGUSR2 = _detail::core::sys::C_SIGUSR2; ///< User-defined signal 2.
    constexpr int SIGPIPE = _detail::core::sys::C_SIGPIPE; ///< Broken pipe.
    constexpr int SIGALRM = _detail::core::sys::C_SIGALRM; ///< Alarm clock.
    constexpr int SIGTERM = _detail::core::sys::C_SIGTERM; ///< Termination request.
    constexpr int SIGSTKFLT = _detail::core::sys::C_SIGSTKFLT; ///< Stack fault (obsolete).
    constexpr int SIGCHLD = _detail::core::sys::C_SIGCHLD; ///< Child terminated or stopped.
    constexpr int SIGCONT = _detail::core::sys::C_SIGCONT; ///< Continue.
    constexpr int SIGSTOP = _detail::core::sys::C_SIGSTOP; ///< Stop, unblockable.
    constexpr int SIGTSTP = _detail::core::sys::C_SIGTSTP; ///< Keyboard stop.
    constexpr int SIGTTIN = _detail::core::sys::C_SIGTTIN; ///< Background read from control terminal.
    constexpr int SIGTTOU = _detail::core::sys::C_SIGTTOU; ///< Background write to control terminal.
    constexpr int SIGURG = _detail::core::sys::C_SIGURG; ///< Urgent data is available at a socket.
    constexpr int SIGXCPU = _detail::core::sys::C_SIGXCPU; ///< CPU time limit exceeded.
    constexpr int SIGXFSZ = _detail::core::sys::C_SIGXFSZ; ///< File size limit exceeded.
    constexpr int SIGVTALRM = _detail::core::sys::C_SIGVTALRM; ///< Virtual timer expired.
    constexpr int SIGPROF = _detail::core::sys::C_SIGPROF; ///< Profiling timer expired.
    constexpr int SIGWINCH = _detail::core::sys::C_SIGWINCH; ///< Window size change (4.3 BSD, Sun).
    constexpr int SIGPOLL = _detail::core::sys::C_SIGPOLL; ///< Pollable event occurred (System V).
    constexpr int SIGPWR = _detail::core::sys::C_SIGPWR; ///< Power failure imminent.
    constexpr int SIGSYS = _detail::core::sys::C_SIGSYS; ///< Bad system call.

    // Historical signals and archaic names for compatibility
    constexpr int SIGIO = _detail::core::sys::C_SIGIO; ///< I/O now possible (4.2 BSD).
    constexpr int SIGIOT = _detail::core::sys::C_SIGIOT; ///< IOT instruction, abort() on a PDP-11.
    constexpr int SIGCLD = _detail::core::sys::C_SIGCLD; ///< Old System V name for SIGCHLD.

    inline const Handler SIG_DFL = _detail::core::sys::C_SIG_DFL; ///< Default signal handler.
    inline const Handler SIG_IGN = _detail::core::sys::C_SIG_IGN; ///< Ignore signal handler.
    inline const Handler SIG_ERR = _detail::core::sys::C_SIG_ERR; ///< Error return value.

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using _detail::core::sys::SigAtomic;
    using _detail::core::sys::SigAtomic_t;
    using _detail::core::sys::SignalAtomic_t;
    #endif 
    
    using _detail::core::sys::SignalAtomic;

    using _detail::core::sys::signal;
    using _detail::core::sys::raise;

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
            return core::sys::signal(sig, handler);
        }

        /**
         * @brief Raises this signal.
         * @return 0 on success, non-zero on failure.
         */
        int raise() const noexcept {
            return core::sys::raise(sig);
        }

        /**
         * @brief Ignores this signal.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        Handler ignore() const noexcept {
            return core::sys::signal(sig, IGNORE);
        }

        /**
         * @brief Resets this signal to default behavior.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        Handler reset() const noexcept {
            return core::sys::signal(sig, DEFAULT);
        }

        // Static methods for direct access
        /**
         * @brief Sets a signal handler for the specified signal.
         * @param sig The signal number.
         * @param handler The signal handler function.
         * @return The previous signal handler, or SIG_ERR on error.
         */
        static Handler handle(int sig, Handler handler) noexcept {
            return core::sys::signal(sig, handler);
        }

        /**
         * @brief Raises the specified signal.
         * @param sig The signal number.
         * @return 0 on success, non-zero on failure.
         */
        static int raise(int sig) noexcept {
            return core::sys::raise(sig);
        }
    };
}
