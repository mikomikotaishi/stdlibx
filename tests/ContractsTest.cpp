#include <version>

import stdx;

using namespace stdx::test;

#if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
/*
 * Functions under contract, deliberately violable from the tests. lossy_half's
 * postcondition only holds for even inputs, so odd inputs violate it.
 */
[[nodiscard]]
i32 checked_divide(
    const i32 a,
    const i32 b
)
    pre (b != 0)
{
    return a / b;
}


[[nodiscard]]
i32 lossy_half(const i32 x)
    post (r: r * 2 == x)
{
    return x / 2;
}

void test_satisfied_contracts_pass() {
    expect_eq(checked_divide(10, 2), 5);
    expect_eq(lossy_half(4), 2);
}

void test_detects_a_violated_precondition() {
    expect_contract_violation([] -> void { i32 _ = checked_divide(1, 0); });
    expect_eq(checked_divide(9, 3), 3, "the test continues after a trapped violation");
}

void test_matches_the_assertion_kind() {
    expect_contract_violation(AssertionKind::PRE, [] -> void { i32 _ = checked_divide(1, 0); });
    expect_contract_violation(AssertionKind::POST, [] -> void { i32 _ = lossy_half(3); });
}

void test_matches_the_predicate_text() {
    expect_contract_violation("b != 0", [] -> void { i32 _ = checked_divide(1, 0); });
}

void test_matches_kind_and_predicate_together() {
    expect_contract_violation(AssertionKind::POST, "r * 2 == x", [] -> void { i32 _ = lossy_half(3); });
}

void test_traps_contract_assert() {
    expect_contract_violation(AssertionKind::ASSERT, [] -> void { contract_assert(2 + 2 == 5); });
}
#endif

int main(int argc, char* argv[]) {
    #if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
    return run(argc, argv, {
        {"Contracts.satisfied_contracts_pass", test_satisfied_contracts_pass},
        {"Contracts.detects_a_violated_precondition", test_detects_a_violated_precondition},
        {"Contracts.matches_the_assertion_kind", test_matches_the_assertion_kind},
        {"Contracts.matches_the_predicate_text", test_matches_the_predicate_text},
        {"Contracts.matches_kind_and_predicate_together", test_matches_kind_and_predicate_together},
        {"Contracts.traps_contract_assert", test_traps_contract_assert},
    });
    #else
    System::out.println("Contracts tests skipped (toolchain has no contract support).");
    #endif
}
