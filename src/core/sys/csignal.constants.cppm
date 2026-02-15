/**
 * @file csignal.constants.cppm
 * @module core:sys.csignal.constants
 * @brief Module file for constants of the standard library C-style signal operations.
 *
 * This file contains the implementation of the C-style signal operations in the standard library.
 */

module;

export module core:sys.csignal.constants;

import :sys.csignal.signal;

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    constexpr int SIGHUP = Signal::HANGUP; ///< Hangup.
    constexpr int SIGINT = Signal::INTERRUPT; ///< Interactive attention signal.
    constexpr int SIGQUIT = Signal::QUIT; ///< Quit.
    constexpr int SIGILL = Signal::ILLEGAL; ///< Illegal instruction.
    constexpr int SIGTRAP = Signal::TRAP; ///< Trace/breakpoint trap.
    constexpr int SIGABRT = Signal::ABORT; ///< Abnormal termination.
    constexpr int SIGBUS = Signal::BUS_ERROR; ///< Bus error.
    constexpr int SIGFPE = Signal::FLOATING_POINT_EXCEPTION; ///< Erroneous arithmetic operation.
    constexpr int SIGKILL = Signal::KILL; ///< Killed.
    constexpr int SIGUSR1 = Signal::USER_DEFINED_1; ///< User-defined signal 1.
    constexpr int SIGSEGV = Signal::SEGMENTATION_VIOLATION; ///< Invalid access to storage.
    constexpr int SIGUSR2 = Signal::USER_DEFINED_2; ///< User-defined signal 2.
    constexpr int SIGPIPE = Signal::BROKEN_PIPE; ///< Broken pipe.
    constexpr int SIGALRM = Signal::ALARM; ///< Alarm clock.
    constexpr int SIGTERM = Signal::TERMINATE; ///< Termination request.
    constexpr int SIGSTKFLT = Signal::STACK_FAULT; ///< Stack fault (obsolete).
    constexpr int SIGCHLD = Signal::CHILD; ///< Child terminated or stopped.
    constexpr int SIGCONT = Signal::CONTINUE; ///< Continue.
    constexpr int SIGSTOP = Signal::STOP; ///< Stop, unblockable.
    constexpr int SIGTSTP = Signal::TERMINAL_STOP; ///< Keyboard stop.
    constexpr int SIGTTIN = Signal::TERMINAL_INPUT; ///< Background read from control terminal.
    constexpr int SIGTTOU = Signal::TERMINAL_OUTPUT; ///< Background write to control terminal.
    constexpr int SIGURG = Signal::URGENT; ///< Urgent data is available at a socket.
    constexpr int SIGXCPU = Signal::CPU_LIMIT; ///< CPU time limit exceeded.
    constexpr int SIGXFSZ = Signal::FILE_SIZE_LIMIT; ///< File size limit exceeded.
    constexpr int SIGVTALRM = Signal::VIRTUAL_ALARM; ///< Virtual timer expired.
    constexpr int SIGPROF = Signal::PROFILING; ///< Profiling timer expired.
    constexpr int SIGWINCH = Signal::WINDOW_CHANGE; ///< Window size change (4.3 BSD, Sun).
    constexpr int SIGPOLL = Signal::POLL; ///< Pollable event occurred (System V).
    constexpr int SIGPWR = Signal::POWER_FAILURE; ///< Power failure imminent.
    constexpr int SIGSYS = Signal::BAD_SYSTEM_CALL; ///< Bad system call.

    // Historical signals and archaic names for compatibility
    constexpr int SIGIO = Signal::IO; ///< I/O now possible (4.2 BSD).
    constexpr int SIGIOT = Signal::IOT; ///< IOT instruction, abort() on a PDP-11.
    constexpr int SIGCLD = Signal::CHILD; ///< Old System V name for SIGCHLD.

    inline const Handler SIG_DFL = Signal::DEFAULT; ///< Default signal handler.
    inline const Handler SIG_IGN = Signal::IGNORE; ///< Ignore signal handler.
    inline const Handler SIG_ERR = Signal::ERROR; ///< Error return value.
}
