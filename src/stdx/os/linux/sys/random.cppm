/**
 * @file random.cppm
 * @module stdx:os.linux.sys.random
 * @brief Module file for Unix random number retrieval operations.
 *
 * This file contains the implementation of the Unix random number retrieval operations from the POSIX libraries,
 * located in <sys/random.h>.
 */

module;

#ifdef __linux__
#include <sys/random.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.random;
#else
export module stdlibx:os.linux.sys.random;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    using ::getrandom;
    using ::getentropy;
    #endif
}
