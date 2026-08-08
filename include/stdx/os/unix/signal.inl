#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX signal operations.
 */
export namespace stdx::os::unix {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<signal.h>)
    using ::kill;
    using ::killpg;
    using ::sigaction;
    using ::sigprocmask;
    using ::sigpending;
    using ::sigsuspend;
    using ::sigwait;
    // Darwin implements POSIX signals but not the realtime-signal extension.
    #ifndef __APPLE__
    using ::sigqueue;
    #endif
    using ::pthread_kill;
    using ::pthread_sigmask;
    #endif
}
