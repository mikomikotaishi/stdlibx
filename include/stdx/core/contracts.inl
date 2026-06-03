#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_contracts
    class [[nodiscard]] AssertionKind final {
    public:
        using Self = std::contracts::assertion_kind;

        static constexpr Self PRE = std::contracts::assertion_kind::pre;
        static constexpr Self POST = std::contracts::assertion_kind::post;
        static constexpr Self ASSERT = std::contracts::assertion_kind::assert;
    private:
        const Self value;
    public:
        constexpr AssertionKind() noexcept = delete;

        constexpr AssertionKind(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] EvaluationSemantic final {
    public:
        using Self = std::contracts::evaluation_semantic;

        static constexpr Self IGNORE = std::contracts::evaluation_semantic::ignore;
        static constexpr Self OBSERVE = std::contracts::evaluation_semantic::observe;
        static constexpr Self ENFORCE = std::contracts::evaluation_semantic::enforce;
        static constexpr Self QUICK_ENFORCE = std::contracts::evaluation_semantic::quick_enforce;
    private:
        const Self value;
    public:
        constexpr EvaluationSemantic() noexcept = default;

        constexpr EvaluationSemantic(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] DetectionMode final {
    public:
        using Self = std::contracts::detection_mode;

        static constexpr Self PREDICATE_FALSE = std::contracts::detection_mode::predicate_false;
        static constexpr Self EVALUATION_EXCEPTION = std::contracts::detection_mode::evaluation_exception;
    private:
        const Self value;
    public:
        constexpr DetectionMode() noexcept = default;

        constexpr DetectionMode(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    using ContractViolation = std::contracts::contract_violation;

    using std::contracts::invoke_default_contract_violation_handler;
    #endif
}
