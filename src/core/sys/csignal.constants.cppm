/**
 * @internal
 * @file csignal.constants.cppm
 * @module core:sys.csignal.constants
 * @brief Module file for constants of the standard library C-style signal operations.
 *
 * This file contains the implementation of the constants of the C-style signal operations in the standard library.
 */

module;

#include <csignal>

// Define fallback values for POSIX signals if not provided by the platform
#ifndef SIGHUP
#define SIGHUP 1
#endif

#ifndef SIGQUIT
#define SIGQUIT 3
#endif

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

#ifndef SIGBUS
#define SIGBUS 7
#endif

#ifndef SIGKILL
#define SIGKILL 9
#endif

#ifndef SIGUSR1
#define SIGUSR1 10
#endif

#ifndef SIGUSR2
#define SIGUSR2 12
#endif

#ifndef SIGPIPE
#define SIGPIPE 13
#endif

#ifndef SIGALRM
#define SIGALRM 14
#endif

#ifndef SIGSTKFLT
#define SIGSTKFLT 16
#endif

#ifndef SIGCHLD
#define SIGCHLD 17
#endif

#ifndef SIGCONT
#define SIGCONT 18
#endif

#ifndef SIGSTOP
#define SIGSTOP 19
#endif

#ifndef SIGTSTP
#define SIGTSTP 20
#endif

#ifndef SIGTTIN
#define SIGTTIN 21
#endif

#ifndef SIGTTOU
#define SIGTTOU 22
#endif

#ifndef SIGURG
#define SIGURG 23
#endif

#ifndef SIGXCPU
#define SIGXCPU 24
#endif

#ifndef SIGXFSZ
#define SIGXFSZ 25
#endif

#ifndef SIGVTALRM
#define SIGVTALRM 26
#endif

#ifndef SIGPROF
#define SIGPROF 27
#endif

#ifndef SIGWINCH
#define SIGWINCH 28
#endif

#ifndef SIGPOLL
#define SIGPOLL 29
#endif

#ifndef SIGPWR
#define SIGPWR 30
#endif

#ifndef SIGSYS
#define SIGSYS 31
#endif

#ifndef SIGIO
#define SIGIO SIGPOLL
#endif

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif

export module core:sys.csignal.constants;

/**
 * @internal
 * @namespace _detail::core::sys
 * @brief Wrapper namespace for standard library system operations.
 * Detail namespace - not to be exported for external use
 */
export namespace _detail::core::sys {
    using Handler = void (*)(int);
    
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using SigAtomic = std::sig_atomic_t;
    using SigAtomic_t = std::sig_atomic_t;
    using SignalAtomic_t = std::sig_atomic_t;
    #endif 
    
    using SignalAtomic = std::sig_atomic_t;

    using std::signal;
    using std::raise;

    // C Standard signals (guaranteed by ISO C)
    constexpr int C_SIGABRT = SIGABRT; ///< Abnormal termination.
    constexpr int C_SIGFPE = SIGFPE; ///< Erroneous arithmetic operation.
    constexpr int C_SIGILL = SIGILL; ///< Illegal instruction.
    constexpr int C_SIGINT = SIGINT; ///< Interactive attention signal.
    constexpr int C_SIGSEGV = SIGSEGV; ///< Invalid access to storage.
    constexpr int C_SIGTERM = SIGTERM; ///< Termination request.

    // POSIX signals
    constexpr int C_SIGHUP = SIGHUP; ///< Hangup.
    constexpr int C_SIGQUIT = SIGQUIT; ///< Quit.
    constexpr int C_SIGTRAP = SIGTRAP; ///< Trace/breakpoint trap.
    constexpr int C_SIGBUS = SIGBUS; ///< Bus error.
    constexpr int C_SIGKILL = SIGKILL; ///< Killed.
    constexpr int C_SIGUSR1 = SIGUSR1; ///< User-defined signal 1.
    constexpr int C_SIGUSR2 = SIGUSR2; ///< User-defined signal 2.
    constexpr int C_SIGPIPE = SIGPIPE; ///< Broken pipe.
    constexpr int C_SIGALRM = SIGALRM; ///< Alarm clock.
    constexpr int C_SIGSTKFLT = SIGSTKFLT; ///< Stack fault (obsolete).
    constexpr int C_SIGCHLD = SIGCHLD; ///< Child terminated or stopped.
    constexpr int C_SIGCONT = SIGCONT; ///< Continue.
    constexpr int C_SIGSTOP = SIGSTOP; ///< Stop, unblockable.
    constexpr int C_SIGTSTP = SIGTSTP; ///< Keyboard stop.
    constexpr int C_SIGTTIN = SIGTTIN; ///< Background read from control terminal.
    constexpr int C_SIGTTOU = SIGTTOU; ///< Background write to control terminal.
    constexpr int C_SIGURG = SIGURG; ///< Urgent data is available at a socket.
    constexpr int C_SIGXCPU = SIGXCPU; ///< CPU time limit exceeded.
    constexpr int C_SIGXFSZ = SIGXFSZ; ///< File size limit exceeded.
    constexpr int C_SIGVTALRM = SIGVTALRM; ///< Virtual timer expired.
    constexpr int C_SIGPROF = SIGPROF; ///< Profiling timer expired.
    constexpr int C_SIGWINCH = SIGWINCH; ///< Window size change (4.3 BSD, Sun).
    constexpr int C_SIGPOLL = SIGPOLL; ///< Pollable event occurred (System V).
    constexpr int C_SIGPWR = SIGPWR; ///< Power failure imminent.
    constexpr int C_SIGSYS = SIGSYS; ///< Bad system call.

    // Historical signals and archaic names for compatibility
    constexpr int C_SIGIO = SIGIO; ///< I/O now possible (4.2 BSD).
    constexpr int C_SIGIOT = SIGIOT; ///< IOT instruction, abort() on a PDP-11.
    constexpr int C_SIGCLD = SIGCLD; ///< Old System V name for SIGCHLD.

    inline const Handler C_SIG_DFL = SIG_DFL; ///< Default signal handler.
    inline const Handler C_SIG_IGN = SIG_IGN; ///< Ignore signal handler.
    inline const Handler C_SIG_ERR = SIG_ERR; ///< Error return value.
}
