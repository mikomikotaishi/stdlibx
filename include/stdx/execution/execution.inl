#pragma once

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

    class Policy final {
    public:
        Policy() = delete;

        static constexpr SequencedPolicy Sequenced = SEQUENCED;
        static constexpr ParallelPolicy Parallel = PARALLEL;
        static constexpr ParallelUnsequencedPolicy ParallelUnsequenced = PARALLEL_UNSEQUENCED;
        static constexpr UnsequencedPolicy Unsequenced = UNSEQUENCED;
    };
}
