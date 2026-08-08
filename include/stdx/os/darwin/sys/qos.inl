#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Quality of service classes - <sys/qos.h> and <pthread/qos.h>.
 *
 * A real-time audio thread should be promoted with
 * pthread_set_qos_class_self_np before it is given a time constraint policy.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<pthread/qos.h>)
    using QosClass = ::qos_class_t;

    using ::qos_class_self;
    using ::qos_class_main;
    using ::pthread_attr_set_qos_class_np;
    using ::pthread_attr_get_qos_class_np;
    using ::pthread_set_qos_class_self_np;
    using ::pthread_get_qos_class_np;
    using ::pthread_override_qos_class_start_np;
    using ::pthread_override_qos_class_end_np;
    #endif
}
