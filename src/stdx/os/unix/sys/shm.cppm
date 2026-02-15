/**
 * @file shm.cppm
 * @module stdx:os.unix.sys.shm
 * @brief Module file for Unix XSI shared memory operations.
 *
 * This file contains the implementation of the Unix XSI shared memory operations from the POSIX libraries,
 * located in <sys/shm.h>.
 */

module;

#ifdef __unix__
#include <sys/shm.h>
#endif

export module stdx:os.unix.sys.shm;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::shmctl;
    using ::shmget;
    using ::shmat;
    using ::shmdt;
    #endif
}
