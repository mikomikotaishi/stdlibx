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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.shm;
#else
export module stdlibx:os.unix.sys.shm;
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
    using ::shmctl;
    using ::shmget;
    using ::shmat;
    using ::shmdt;
    #endif
}
