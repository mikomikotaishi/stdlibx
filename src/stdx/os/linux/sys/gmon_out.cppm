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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.gmon_out;
#else
export module stdlibx:os.linux.sys.gmon_out;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using GmonHdr = ::gmon_hdr;
    using GmonHistHdr = ::gmon_hist_hdr;
    #endif

    using GmonHeader = ::gmon_hdr;
    using GmonHistoryHeader = ::gmon_hist_hdr;
    using GmonCgArcRecord = ::gmon_cg_arc_record;
    #endif
}
