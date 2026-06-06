#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/gmon_out.h>)
    using GmonHeader = ::gmon_hdr;
    using GmonHistoryHeader = ::gmon_hist_hdr;
    using GmonCgArcRecord = ::gmon_cg_arc_record;
    #endif
}
