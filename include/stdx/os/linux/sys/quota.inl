#pragma once

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
