/**
 * @file contracts.cppm
 * @module stdx:core.contracts
 * @brief Module file for (experimental) standard library contract operations.
 *
 * This file contains the implementation of the (experimental) contract operations in the standard library.
 */


module;

#include "Macros.hpp"

export module stdx:core.contracts;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::core {
    #if __has_include(<experimental/contract>)
    using ::core::ContractViolationContinuationMode;
    using ::core::ContractViolation;
    #endif
}
