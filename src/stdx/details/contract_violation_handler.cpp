/**
 * @file contract_violation_handler.cpp
 * @brief The replaceable contract-violation handler for test binaries.
 *
 * Linking the stdlibx_test_contracts object library into a test executable
 * replaces the default contract-violation handler with this one, which routes
 * every violation into the test framework (see stdx::test::on_contract_violation).
 * It is an object library rather than an archive member so the definition is
 * always pulled into the link without being referenced, and it is deliberately
 * not folded into stdlibx itself so ordinary programs keep the default handler.
 *
 * The definition is weak, so a test binary that provides its own (strong)
 * ::handle_contract_violation overrides this one without a duplicate-symbol
 * error; such a handler can call stdx::test::on_contract_violation itself
 * wherever it wants the framework's routing back. Weak still beats the
 * toolchain's default, which lives in a library archive that is only searched
 * for symbols nothing in the link defines.
 */

#include <version>

#if defined(__cpp_contracts) && defined(__cpp_lib_contracts)

import stdx;

[[gnu::weak]]
void handle_contract_violation(const ContractViolation& violation) {
    stdx::test::on_contract_violation(violation);
}
#endif
