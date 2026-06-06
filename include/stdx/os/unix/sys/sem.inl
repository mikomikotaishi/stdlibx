#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if defined(__unix__) && __has_include(<sys/sem.h>)
    using SemaphoreInfo = ::seminfo;
    using SemaphoreBuffer = ::sembuf;

    using ::semctl;
    using ::semget;
    using ::semop;
    using ::semtimedop;
    #endif
}
