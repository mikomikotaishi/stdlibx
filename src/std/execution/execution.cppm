/**
 * @file execution.cppm
 * @module std:execution.execution
 * @brief Module file for standard library execution policy operations.
 *
 * This file contains the implementation of the execution policy operations in the standard library.
 */

module;

#include <execution>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:execution.execution;
#else
export module stdlib:execution.execution;
#endif

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    template <typename T>
    using IsExecutionPolicy = std::is_execution_policy<T>;
}

/**
 * @namespace std::execution
 * @brief Wrapper namespace for standard library execution operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::execution {
#else 
export namespace stdlib::execution {
#endif
    using SequencedPolicy = std::execution::sequenced_policy;
    using ParallelPolicy = std::execution::parallel_policy;
    using ParallelUnsequencedPolicy = std::execution::parallel_unsequenced_policy;
    using UnsequencedPolicy = std::execution::unsequenced_policy;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    inline constexpr SequencedPolicy SEQ = std::execution::seq;
    inline constexpr ParallelPolicy PAR = std::execution::par;
    inline constexpr ParallelUnsequencedPolicy PAR_UNSEQ = std::execution::par_unseq;
    inline constexpr UnsequencedPolicy UNSEQ = std::execution::unseq;
    #endif

    inline constexpr SequencedPolicy SEQUENCED = std::execution::seq;
    inline constexpr ParallelPolicy PARALLEL = std::execution::par;
    inline constexpr ParallelUnsequencedPolicy PARALLEL_UNSEQUENCED = std::execution::par_unseq;
    inline constexpr UnsequencedPolicy UNSEQUENCED = std::execution::unseq;
}
