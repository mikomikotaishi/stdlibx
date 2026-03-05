#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using MessageBuffer = ::msgbuf;

    using ::msgctl;
    using ::msgget;
    using ::msgrcv;
    using ::msgsnd;
    #endif
}
