#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Thread scheduling policies - <mach/thread_policy.h>.
 *
 * A CoreAudio render thread is configured by handing
 * ThreadTimeConstraintPolicy to thread_policy_set with
 * THREAD_TIME_CONSTRAINT_POLICY.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/thread_policy.h>)
    using ThreadPolicyFlavor = ::thread_policy_flavor_t;
    using ThreadPolicy = ::thread_policy_t;
    using ThreadStandardPolicy = ::thread_standard_policy_data_t;
    using ThreadExtendedPolicy = ::thread_extended_policy_data_t;
    using ThreadTimeConstraintPolicy = ::thread_time_constraint_policy_data_t;
    using ThreadPrecedencePolicy = ::thread_precedence_policy_data_t;
    using ThreadAffinityPolicy = ::thread_affinity_policy_data_t;
    #endif
}
