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
    class [[nodiscard]] ContractViolationContinuationMode final {
    public:
        using Self = std::experimental::contract_violation_continuation_mode;

        static constexpr Self NEVER_CONTINUE = std::experimental::contract_violation_continuation_mode::never_continue;
        static constexpr Self MAYBE_CONTINUE = std::experimental::contract_violation_continuation_mode::maybe_continue;
    private:
        Self value;
    public:
        constexpr ContractViolationContinuationMode(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using ContractViolation = std::experimental::contract_violation;
    #endif
}
