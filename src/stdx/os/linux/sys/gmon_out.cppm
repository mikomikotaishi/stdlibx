/**
 * @file gmon_out.cppm
 * @module stdx:os.linux.sys.gmon_out
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/gmon_out.h>.
 */

module;

#ifdef __linux__
#include <sys/gmon_out.h>
#endif

export module stdx:os.linux.sys.gmon_out;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using GmonHeader = ::gmon_hdr;
    using GmonHistoryHeader = ::gmon_hist_hdr;
    using GmonCgArcRecord = ::gmon_cg_arc_record;
    #endif
}
