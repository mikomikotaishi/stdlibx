#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_execution
    template <typename T>
    using IsExecutionPolicy = std::is_execution_policy<T>;
    #endif
}

/**
 * @namespace std::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    #ifdef __cpp_lib_execution
    using SequencedPolicy = std::execution::sequenced_policy;
    using ParallelPolicy = std::execution::parallel_policy;
    using ParallelUnsequencedPolicy = std::execution::parallel_unsequenced_policy;
    using UnsequencedPolicy = std::execution::unsequenced_policy;

    class Policy final {
    public:
        Policy() = DELETE_METHOD("Policy is a static utility class and cannot be instantiated.");

        static constexpr SequencedPolicy SEQUENCED = std::execution::seq;
        static constexpr ParallelPolicy PARALLEL = std::execution::par;
        static constexpr ParallelUnsequencedPolicy PARALLEL_UNSEQUENCED = std::execution::par_unseq;
        static constexpr UnsequencedPolicy UNSEQUENCED = std::execution::unseq;
    };
    #endif
}
