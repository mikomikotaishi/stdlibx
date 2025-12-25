/**
 * @file condition_variable.cppm
 * @module std:sync.condition_variable
 * @brief Module file for standard library condition variable operations.
 *
 * This file contains the implementation of the condition variable operations in the standard library.
 */

module;

#include <condition_variable>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.condition_variable;
#else
export module stdlib:sync.condition_variable;
#endif

/**
 * @namespace std::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::sync {
#else 
export namespace stdlib::sync {
#endif
    using ConditionVariable = std::condition_variable;
    using ConditionVariableAny = std::condition_variable_any;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using CvStatus = std::cv_status;
    #endif

    using ConditionVariableStatus = std::cv_status;

    using std::notify_all_at_thread_exit;
}
