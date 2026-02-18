#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::core {
    #if __has_include(<experimental/contract>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
    using ::core::ContractViolationContinuationMode;
    using ::core::ContractViolation;
    #endif
}