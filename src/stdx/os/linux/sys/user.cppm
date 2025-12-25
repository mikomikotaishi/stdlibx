/**
 * @file user.cppm
 * @module stdx:os.linux.sys.user
 * @brief Module file for Unix GDB user operations.
 *
 * This file contains the implementation of the Unix GDB user operations from the POSIX libraries,
 * located in <sys/user.h>.
 */

module;

#ifdef __linux__
#include <sys/user.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.user;
#else
export module stdlibx:os.linux.sys.user;
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
    using UserFpRegsStruct = ::user_fpregs_struct;
    using UserRegsStruct = ::user_regs_struct;
    using User = ::user;
    #endif
}
