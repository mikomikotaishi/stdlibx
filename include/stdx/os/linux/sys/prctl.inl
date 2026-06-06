#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/prctl.h>)
    using ::prctl;

    /// prctl() option: set the calling process's parent-death signal. Named with
    /// a suffix because PR_SET_PDEATHSIG is itself a macro in <sys/prctl.h>.
    inline constexpr int PR_SET_PDEATHSIG_OPTION = PR_SET_PDEATHSIG;
    #endif
}
