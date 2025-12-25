/**
 * @file ptrace.cppm
 * @module stdx:os.linux.sys.ptrace
 * @brief Module file for Unix ptrace debugger interface operations.
 *
 * This file contains the implementation of the Unix ptrace debugger interface operations from the POSIX libraries,
 * located in <sys/ptrace.h>.
 */

module;

#ifdef __linux__
#include <sys/ptrace.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.ptrace;
#else
export module stdlibx:os.linux.sys.ptrace;
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
    using ::ptrace;
    #endif
}
