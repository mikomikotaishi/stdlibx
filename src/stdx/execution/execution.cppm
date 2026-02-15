/**
 * @file execution.cppm
 * @module stdx:execution.execution
 * @brief Module file for standard library execution policy operations.
 *
 * This file contains the implementation of the execution policy operations in the standard library.
 */

module;

#include <execution>

export module stdx:execution.execution;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T>
    using IsExecutionPolicy = std::is_execution_policy<T>;
}

/**
 * @namespace std::execution
 * @brief Wrapper namespace for standard library execution operations.
 */
export namespace stdx::execution {
    using SequencedPolicy = std::execution::sequenced_policy;
    using ParallelPolicy = std::execution::parallel_policy;
    using ParallelUnsequencedPolicy = std::execution::parallel_unsequenced_policy;
    using UnsequencedPolicy = std::execution::unsequenced_policy;

    inline constexpr SequencedPolicy SEQUENCED = std::execution::seq;
    inline constexpr ParallelPolicy PARALLEL = std::execution::par;
    inline constexpr ParallelUnsequencedPolicy PARALLEL_UNSEQUENCED = std::execution::par_unseq;
    inline constexpr UnsequencedPolicy UNSEQUENCED = std::execution::unseq;
}
