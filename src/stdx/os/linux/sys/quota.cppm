/**
 * @file quota.cppm
 * @module stdx:os.linux.sys.quota
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/quota.h>.
 */

module;

#ifdef __linux__
#include <sys/quota.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.quota;
#else
export module stdlibx:os.linux.sys.quota;
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
    using IfDqBlk = ::if_dqblk;
    using IfNextDqBlk = ::if_nextdqblk;
    using IfDqInfo = ::if_dqinfo;

    using DqBlk = ::dqblk;
    using DqInfo = ::dqinfo;
    #endif

    using InformationDefaultQuotaBlock = ::if_dqblk;
    using InformationNextDefaultQuotaBlock = ::if_nextdqblk;
    using InformationDefaultQuotaInfo = ::if_dqinfo;

    using DefaultQuotaBlock = ::dqblk;
    using DefaultQuotaInfo = ::dqinfo;

    using ::quotactl;
    #endif
}
