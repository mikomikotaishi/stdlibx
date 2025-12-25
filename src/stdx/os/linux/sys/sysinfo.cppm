/**
 * @file sysinfo.cppm
 * @module stdx:os.linux.sys.sysinfo
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/sysinfo.h>.
 */

module;

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.sysinfo;
#else
export module stdlibx:os.linux.sys.sysinfo;
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
    using ::sysinfo;
    using ::get_nprocs_conf;
    using ::get_nprocs;
    using ::get_phys_pages;
    using ::get_avphys_pages;
    #endif
}
