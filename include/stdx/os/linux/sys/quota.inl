#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/quota.h>)
    using InformationDefaultQuotaBlock = ::if_dqblk;
    using InformationNextDefaultQuotaBlock = ::if_nextdqblk;
    using InformationDefaultQuotaInfo = ::if_dqinfo;

    using DefaultQuotaBlock = ::dqblk;
    using DefaultQuotaInfo = ::dqinfo;

    using ::quotactl;
    #endif
}
