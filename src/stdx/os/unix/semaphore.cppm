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

export module stdx:os.unix.semaphore;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
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
