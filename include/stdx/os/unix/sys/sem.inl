#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using SemaphoreInfo = ::seminfo;
    using SemaphoreBuffer = ::sembuf;

    using ::semctl;
    using ::semget;
    using ::semop;
    using ::semtimedop;
    #endif
}
