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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.sem;
#else
export module stdlibx:os.unix.sys.sem;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__

    using SemaphoreInfo = ::seminfo;
    using SemaphoreBuffer = ::sembuf;

    using ::semctl;
    using ::semget;
    using ::semop;
    using ::semtimedop;
    #endif
}
