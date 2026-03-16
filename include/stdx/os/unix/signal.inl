#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX signal operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
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
