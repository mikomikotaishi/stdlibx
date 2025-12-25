/**
 * @file contracts.cppm
 * @module core:contracts
 * @brief Module file for (experimental) standard library contract operations.
 *
 * This file contains the implementation of the (experimental) contract operations in the standard library.
 */


module;

#if __has_include(<experimental/contract>)
#include <experimental/contract>
#endif

#include "Macros.hpp"

export module core:contracts;

/**
 * @namespace core
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace core {
    #if __has_include(<experimental/contract>)
    class ContractViolationContinuationMode {
    public:
        using InternalContractViolationContinuationMode = std::experimental::contract_violation_continuation_mode;

        ContractViolationContinuationMode() = delete;

        static constexpr InternalContractViolationContinuationMode NEVER_CONTINUE = std::experimental::contract_violation_continuation_mode::never_continue;
        static constexpr InternalContractViolationContinuationMode MAYBE_CONTINUE = std::experimental::contract_violation_continuation_mode::maybe_continue;
    };

    using ContractViolation = std::experimental::contract_violation;
    #endif
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif

