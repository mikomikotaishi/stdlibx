/**
 * @file constants.cppm
 * @module core:constants
 * @brief Module file for constants of the standard library.
 *
 * This file contains the implementation of constants in the standard library.
 */

module;

#include <cmath>
#include <csignal>
#include <cstdlib>
#include <functional>

export module core:constants;

/**
 * @internal
 * @namespace _detail
 * @brief Detail namespace - not to be exported for external use
 */
namespace _detail {
    constexpr auto HugeVal = HUGE_VAL;
    constexpr auto HugeValF = HUGE_VALF;
    constexpr auto HugeValL = HUGE_VALL;
    constexpr auto Infinity = INFINITY;
    constexpr auto NaN = NAN;
    constexpr auto MathErrHandling = math_errhandling;
    constexpr auto MathErrNo = MATH_ERRNO;
    constexpr auto MathErrExcept = MATH_ERREXCEPT;
    constexpr auto FpNormal = FP_NORMAL;
    constexpr auto FpSubnormal = FP_SUBNORMAL;
    constexpr auto FpZero = FP_ZERO;
    constexpr auto FpInfinite = FP_INFINITE;
    constexpr auto FpNaN = FP_NAN;

    constexpr auto ExitSuccess = EXIT_SUCCESS;
    constexpr auto ExitFailure = EXIT_FAILURE;
    const auto MultibyteCurrentMax = MB_CUR_MAX;
    constexpr auto RandMax = RAND_MAX;

    constexpr auto SigHup = SIGHUP;
    constexpr auto SigInt = SIGINT;
    constexpr auto SigQuit = SIGQUIT;
    constexpr auto SigIll = SIGILL;
    constexpr auto SigTrap = SIGTRAP;
    constexpr auto SigAbrt = SIGABRT;
    constexpr auto SigBus = SIGBUS;
    constexpr auto SigFpe = SIGFPE;
    constexpr auto SigKill = SIGKILL;
    constexpr auto SigUsr1 = SIGUSR1;
    constexpr auto SigSegv = SIGSEGV;
    constexpr auto SigUsr2 = SIGUSR2;
    constexpr auto SigPipe = SIGPIPE;
    constexpr auto SigAlrm = SIGALRM;
    constexpr auto SigTerm = SIGTERM;
    constexpr auto SigStkFlt = SIGSTKFLT;
    constexpr auto SigChld = SIGCHLD;
    constexpr auto SigCont = SIGCONT;
    constexpr auto SigStop = SIGSTOP;
    constexpr auto SigTstp = SIGTSTP;
    constexpr auto SigTtin = SIGTTIN;
    constexpr auto SigTtou = SIGTTOU;
    constexpr auto SigUrg = SIGURG;
    constexpr auto SigXcpu = SIGXCPU;
    constexpr auto SigXfsz = SIGXFSZ;
    constexpr auto SigVtAlrm = SIGVTALRM;
    constexpr auto SigProf = SIGPROF;
    constexpr auto SigWinCh = SIGWINCH;
    constexpr auto SigPoll = SIGPOLL;
    constexpr auto SigPwr = SIGPWR;
    constexpr auto SigSys = SIGSYS;
    constexpr auto SigIo = SIGIO;
    constexpr auto SigIot = SIGIOT;
    constexpr auto SigCld = SIGCLD;

    inline const auto SigDfl = SIG_DFL;
    inline const auto SigIgn = SIG_IGN;
    inline const auto SigErr = SIG_ERR;
}

#undef HUGE_VAL
#undef HUGE_VALF
#undef HUGE_VALL
#undef INFINITY
#undef NAN
#undef math_errhandling
#undef MATH_ERRNO
#undef MATH_ERREXCEPT
#undef FP_NORMAL 
#undef FP_SUBNORMAL
#undef FP_ZERO 
#undef FP_INFINITE
#undef FP_NAN

#undef EXIT_SUCCESS
#undef EXIT_FAILURE
#undef MB_CUR_MAX
#undef NULL
#undef RAND_MAX

#undef SIGHUP
#undef SIGINT
#undef SIGQUIT
#undef SIGILL
#undef SIGTRAP
#undef SIGABRT
#undef SIGBUS
#undef SIGFPE
#undef SIGKILL
#undef SIGUSR1
#undef SIGSEGV
#undef SIGUSR2
#undef SIGPIPE
#undef SIGALRM
#undef SIGTERM
#undef SIGSTKFLT
#undef SIGCHLD
#undef SIGCONT
#undef SIGSTOP
#undef SIGTSTP
#undef SIGTTIN
#undef SIGTTOU
#undef SIGURG
#undef SIGXCPU
#undef SIGXFSZ
#undef SIGVTALRM
#undef SIGPROF
#undef SIGWINCH
#undef SIGPOLL
#undef SIGPWR
#undef SIGSYS
#undef SIGIO
#undef SIGIOT
#undef SIGCLD
#undef SIG_DFL
#undef SIG_IGN
#undef SIG_ERR

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    constexpr auto EXIT_SUCCESS = _detail::ExitSuccess;
    constexpr auto EXIT_FAILURE = _detail::ExitFailure;
    const auto MB_CUR_MAX = _detail::MultibyteCurrentMax;
    constexpr void* const NULL = (void*)0;
    constexpr auto RAND_MAX = _detail::RandMax;

    class Placeholders final {
    public:
        Placeholders() = delete;

        static constexpr auto _1 = std::placeholders::_1;
        static constexpr auto _2 = std::placeholders::_2;
        static constexpr auto _3 = std::placeholders::_3;
        static constexpr auto _4 = std::placeholders::_4;
        static constexpr auto _5 = std::placeholders::_5;
        static constexpr auto _6 = std::placeholders::_6;
        static constexpr auto _7 = std::placeholders::_7;
        static constexpr auto _8 = std::placeholders::_8;
        static constexpr auto _9 = std::placeholders::_9;
        static constexpr auto _10 = std::placeholders::_10;
        static constexpr auto _11 = std::placeholders::_11;
        static constexpr auto _12 = std::placeholders::_12;
        static constexpr auto _13 = std::placeholders::_13;
        static constexpr auto _14 = std::placeholders::_14;
        static constexpr auto _15 = std::placeholders::_15;
        static constexpr auto _16 = std::placeholders::_16;
        static constexpr auto _17 = std::placeholders::_17;
        static constexpr auto _18 = std::placeholders::_18;
        static constexpr auto _19 = std::placeholders::_19;
        static constexpr auto _20 = std::placeholders::_20;
        static constexpr auto _21 = std::placeholders::_21;
        static constexpr auto _22 = std::placeholders::_22;
        static constexpr auto _23 = std::placeholders::_23;
        static constexpr auto _24 = std::placeholders::_24;
        static constexpr auto _25 = std::placeholders::_25;
        static constexpr auto _26 = std::placeholders::_26;
        static constexpr auto _27 = std::placeholders::_27;
        static constexpr auto _28 = std::placeholders::_28;
        static constexpr auto _29 = std::placeholders::_29;
    };

    /**
     * @namespace math
     * @brief Wrapper namespace for standard library mathematical operations.
     */
    namespace math {
        constexpr auto HUGE_VAL = _detail::HugeVal;
        constexpr auto HUGE_VALF = _detail::HugeValF;
        constexpr auto HUGE_VALL = _detail::HugeValL;
        constexpr auto INFINITY = _detail::Infinity;
        constexpr auto NAN = _detail::NaN;
        constexpr auto MATH_ERRHANDLING = _detail::MathErrHandling;
        constexpr auto MATH_ERRNO = _detail::MathErrNo;
        constexpr auto MATH_ERREXCEPT = _detail::MathErrExcept;
        constexpr auto FP_NORMAL = _detail::FpNormal;
        constexpr auto FP_SUBNORMAL = _detail::FpSubnormal;
        constexpr auto FP_ZERO = _detail::FpZero;
        constexpr auto FP_INFINITE = _detail::FpInfinite;
        constexpr auto FP_NAN = _detail::FpNaN;
    }

    /**
     * @namespace sys
     * @brief Wrapper namespace for standard library system operations.
     */
    namespace sys {
        constexpr int SIGHUP = _detail::SigHup; ///< Hangup.
        constexpr int SIGINT = _detail::SigInt; ///< Interactive attention signal.
        constexpr int SIGQUIT = _detail::SigQuit; ///< Quit.
        constexpr int SIGILL = _detail::SigIll; ///< Illegal instruction.
        constexpr int SIGTRAP = _detail::SigTrap; ///< Trace/breakpoint trap.
        constexpr int SIGABRT = _detail::SigAbrt; ///< Abnormal termination.
        constexpr int SIGBUS = _detail::SigBus; ///< Bus error.
        constexpr int SIGFPE = _detail::SigFpe; ///< Erroneous arithmetic operation.
        constexpr int SIGKILL = _detail::SigKill; ///< Killed.
        constexpr int SIGUSR1 = _detail::SigUsr1; ///< User-defined signal 1.
        constexpr int SIGSEGV = _detail::SigSegv; ///< Invalid access to storage.
        constexpr int SIGUSR2 = _detail::SigUsr2; ///< User-defined signal 2.
        constexpr int SIGPIPE = _detail::SigPipe; ///< Broken pipe.
        constexpr int SIGALRM = _detail::SigAlrm; ///< Alarm clock.
        constexpr int SIGTERM = _detail::SigTerm; ///< Termination request.
        constexpr int SIGSTKFLT = _detail::SigStkFlt; ///< Stack fault (obsolete).
        constexpr int SIGCHLD = _detail::SigChld; ///< Child terminated or stopped.
        constexpr int SIGCONT = _detail::SigCont; ///< Continue.
        constexpr int SIGSTOP = _detail::SigStop; ///< Stop, unblockable.
        constexpr int SIGTSTP = _detail::SigTstp; ///< Keyboard stop.
        constexpr int SIGTTIN = _detail::SigTtin; ///< Background read from control terminal.
        constexpr int SIGTTOU = _detail::SigTtou; ///< Background write to control terminal.
        constexpr int SIGURG = _detail::SigUrg; ///< Urgent data is available at a socket.
        constexpr int SIGXCPU = _detail::SigXcpu; ///< CPU time limit exceeded.
        constexpr int SIGXFSZ = _detail::SigXfsz; ///< File size limit exceeded.
        constexpr int SIGVTALRM = _detail::SigVtAlrm; ///< Virtual timer expired.
        constexpr int SIGPROF = _detail::SigProf; ///< Profiling timer expired.
        constexpr int SIGWINCH = _detail::SigWinCh; ///< Window size change (4.3 BSD, Sun).
        constexpr int SIGPOLL = _detail::SigPoll; ///< Pollable event occurred (System V).
        constexpr int SIGPWR = _detail::SigPwr; ///< Power failure imminent.
        constexpr int SIGSYS = _detail::SigSys; ///< Bad system call.

        // Historical signals and archaic names for compatibility
        constexpr int SIGIO = _detail::SigIo; ///< I/O now possible (4.2 BSD).
        constexpr int SIGIOT = _detail::SigIot; ///< IOT instruction, abort() on a PDP-11.
        constexpr int SIGCLD = _detail::SigCld; ///< Old System V name for SIGCHLD.

        inline const auto SIG_DFL = _detail::SigDfl; ///< Default signal handler.
        inline const auto SIG_IGN = _detail::SigIgn; ///< Ignore signal handler.
        inline const auto SIG_ERR = _detail::SigErr; ///< Error return value.
    }
}
