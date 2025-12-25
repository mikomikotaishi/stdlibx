/**
 * @file semaphore.cppm
 * @module stdx:os.unix.semaphore
 * @brief Module file for Unix POSIX semaphore operations.
 *
 * This file contains the implementation of the Unix POSIX semaphore operations from the POSIX libraries,
 * located in <semaphore.h>.
 */

module;

#ifdef __unix__
#include <semaphore.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.semaphore;
#else
export module stdlibx:os.unix.semaphore;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::sem_init;
    using ::sem_destroy;
    using ::sem_open;
    using ::sem_close;
    using ::sem_unlink;
    using ::sem_wait;
    using ::sem_timedwait;
    using ::sem_clockwait;
    using ::sem_trywait;
    using ::sem_post;
    using ::sem_getvalue;
    #endif
}
