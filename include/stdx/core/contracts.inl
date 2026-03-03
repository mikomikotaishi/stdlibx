#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_contracts
    using ::core::AssertionKind;
    using ::core::EvaluationSemantic;
    using ::core::DetectionMode;
    using ::core::ContractViolation;

    using ::core::invoke_default_contract_violation_handler;
    #endif
}