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

export module stdx:os.linux.sys.user;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using UserFpRegs = ::user_fpregs_struct;
    using UserRegs = ::user_regs_struct;
    using User = ::user;
    #endif
}
