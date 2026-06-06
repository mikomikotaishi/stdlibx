#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX signal operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<signal.h>)
    using ::kill;
    using ::killpg;
    using ::sigaction;
    using ::sigprocmask;
    using ::sigpending;
    using ::sigsuspend;
    using ::sigwait;
    using ::sigqueue;
    using ::pthread_kill;
    using ::pthread_sigmask;
    #endif
}
