#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::sysinfo;
    using ::get_nprocs_conf;
    using ::get_nprocs;
    using ::get_phys_pages;
    using ::get_avphys_pages;
    #endif
}
