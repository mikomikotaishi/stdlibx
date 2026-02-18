#pragma once

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    using ConditionVariable = std::condition_variable;
    using ConditionVariableAny = std::condition_variable_any;

    using ConditionVariableStatus = std::cv_status;

    using std::notify_all_at_thread_exit;
}
