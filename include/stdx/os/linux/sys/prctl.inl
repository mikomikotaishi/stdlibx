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

    /// prctl() option: make the calling process the one that adopts and reaps its
    /// orphaned descendants, in place of init. Suffixed for the same reason.
    inline constexpr int PR_SET_CHILD_SUBREAPER_OPTION = PR_SET_CHILD_SUBREAPER;
    #endif
}
