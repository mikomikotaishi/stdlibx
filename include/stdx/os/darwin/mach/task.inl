#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Task inspection and control - <mach/task.h> and <mach/task_info.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/task.h>)
    using TaskBasicInfo64 = ::task_basic_info_64_data_t;
    using TaskEventsInfo = ::task_events_info_data_t;
    using TaskThreadTimesInfo = ::task_thread_times_info_data_t;
    using TaskAbsoluteTimeInfo = ::task_absolutetime_info_data_t;
    using TaskVmInfo = ::task_vm_info_data_t;
    using TaskDyldInfo = ::task_dyld_info_data_t;
    using TaskPowerInfo = ::task_power_info_data_t;

    using ::task_info;
    using ::task_threads;
    using ::task_suspend;
    using ::task_resume;
    using ::task_terminate;
    using ::task_get_special_port;
    using ::task_set_special_port;
    using ::task_set_exception_ports;
    using ::task_get_exception_ports;
    using ::task_policy_set;
    using ::task_policy_get;
    #endif
}
