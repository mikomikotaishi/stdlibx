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

export module stdx:os.linux.sys.quota;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using InformationDefaultQuotaBlock = ::if_dqblk;
    using InformationNextDefaultQuotaBlock = ::if_nextdqblk;
    using InformationDefaultQuotaInfo = ::if_dqinfo;

    using DefaultQuotaBlock = ::dqblk;
    using DefaultQuotaInfo = ::dqinfo;

    using ::quotactl;
    #endif
}
