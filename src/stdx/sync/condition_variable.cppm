/**
 * @file condition_variable.cppm
 * @module stdx:sync.condition_variable
 * @brief Module file for standard library condition variable operations.
 *
 * This file contains the implementation of the condition variable operations in the standard library.
 */

module;

#include <condition_variable>

export module stdx:sync.condition_variable;

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
