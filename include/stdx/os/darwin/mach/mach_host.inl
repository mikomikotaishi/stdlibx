#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Host-wide information and statistics - <mach/mach_host.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_host.h>)
    using HostBasicInfo = ::host_basic_info_data_t;
    using HostLoadInfo = ::host_load_info_data_t;
    using HostCpuLoadInfo = ::host_cpu_load_info_data_t;
    using HostSchedInfo = ::host_sched_info_data_t;
    using VmStatistics = ::vm_statistics_data_t;
    using VmStatistics64 = ::vm_statistics64_data_t;

    using ::host_info;
    using ::host_statistics;
    using ::host_statistics64;
    using ::host_get_clock_service;
    using ::host_processor_info;
    using ::host_processors;
    #endif
}
