#pragma once

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
