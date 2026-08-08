#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Thread inspection and control - <mach/thread_act.h> and <mach/thread_info.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/thread_act.h>)
    using ThreadBasicInfo = ::thread_basic_info_data_t;
    using ThreadIdentifierInfo = ::thread_identifier_info_data_t;
    using ThreadExtendedInfo = ::thread_extended_info_data_t;

    using ::thread_info;
    using ::thread_suspend;
    using ::thread_resume;
    using ::thread_abort;
    using ::thread_terminate;
    using ::thread_get_state;
    using ::thread_set_state;
    using ::thread_policy_set;
    using ::thread_policy_get;
    using ::thread_set_exception_ports;
    using ::thread_get_exception_ports;
    #endif
}
