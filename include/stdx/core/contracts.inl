#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_contracts
    using ::core::prelude::AssertionKind;
    using ::core::prelude::EvaluationSemantic;
    using ::core::prelude::DetectionMode;
    using ::core::prelude::ContractViolation;

    using ::core::prelude::invoke_default_contract_violation_handler;
    #endif
}