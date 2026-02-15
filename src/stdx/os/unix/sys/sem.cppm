/**
 * @file sem.cppm
 * @module stdx:os.unix.sys.sem
 * @brief Module file for Unix XSI semaphore operations.
 *
 * This file contains the implementation of the Unix XSI semaphore operations from the POSIX libraries,
 * located in <sys/sem.h>.
 */

module;

#ifdef __unix__
#include <sys/sem.h>
#endif

export module stdx:os.unix.sys.sem;

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
