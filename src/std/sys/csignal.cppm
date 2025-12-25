/**
 * @file csignal.cppm
 * @module std:sys.csignal
 * @brief Module file for standard library C-style signal operations.
 *
 * This file contains the implementation of the C-style signal operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sys.csignal;
#else
export module stdlib:sys.csignal;
#endif

import core;

/**
 * @namespace std::sys
 * @brief Wrapper namespace for standard library system operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::sys {
#else 
export namespace stdlib::sys {
#endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::sys::SigAtomic;
    using core::sys::SigAtomic_t;
    using core::sys::SignalAtomic_t;
    #endif 
    
    using core::sys::SignalAtomic;

    using core::sys::signal;
    using core::sys::raise;

    using core::sys::Signal;

    using core::sys::SIG_DFL; ///< Default signal handler.
    using core::sys::SIG_IGN; ///< Ignore signal handler.
    using core::sys::SIG_ERR; ///< Error return value.

    using core::sys::SIGHUP; ///< Hangup.
    using core::sys::SIGINT; ///< Interactive attention signal.
    using core::sys::SIGQUIT; ///< Quit.
    using core::sys::SIGILL; ///< Illegal instruction.
    using core::sys::SIGTRAP; ///< Trace/breakpoint trap.
    using core::sys::SIGABRT; ///< Abnormal termination.
    using core::sys::SIGBUS; ///< Bus error.
    using core::sys::SIGFPE; ///< Erroneous arithmetic operation.
    using core::sys::SIGKILL; ///< Killed.
    using core::sys::SIGUSR1; ///< User-defined signal 1.
    using core::sys::SIGSEGV; ///< Invalid access to storage.
    using core::sys::SIGUSR2; ///< User-defined signal 2.
    using core::sys::SIGPIPE; ///< Broken pipe.
    using core::sys::SIGALRM; ///< Alarm clock.
    using core::sys::SIGTERM; ///< Termination request.
    using core::sys::SIGSTKFLT; ///< Stack fault (obsolete).
    using core::sys::SIGCHLD; ///< Child terminated or stopped.
    using core::sys::SIGCONT; ///< Continue.
    using core::sys::SIGSTOP; ///< Stop, unblockable.
    using core::sys::SIGTSTP; ///< Keyboard stop.
    using core::sys::SIGTTIN; ///< Background read from control terminal.
    using core::sys::SIGTTOU; ///< Background write to control terminal.
    using core::sys::SIGURG; ///< Urgent data is available at a socket.
    using core::sys::SIGXCPU; ///< CPU time limit exceeded.
    using core::sys::SIGXFSZ; ///< File size limit exceeded.
    using core::sys::SIGVTALRM; ///< Virtual timer expired.
    using core::sys::SIGPROF; ///< Profiling timer expired.
    using core::sys::SIGWINCH; ///< Window size change (4.3 BSD, Sun).
    using core::sys::SIGPOLL; ///< Pollable event occurred (System V).
    using core::sys::SIGPWR; ///< Power failure imminent.
    using core::sys::SIGSYS; ///< Bad system call.

    /* Historical signals and archaic names for compatibility */
    using core::sys::SIGIO; ///< I/O now possible (4.2 BSD).
    using core::sys::SIGIOT; ///< IOT instruction, abort() on a PDP-11.
    using core::sys::SIGCLD; ///< Old System V name for SIGCHLD.
}
