#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/user.h>)
    using UserFpRegs = ::user_fpregs_struct;
    using UserRegs = ::user_regs_struct;
    using User = ::user;
    #endif
}
