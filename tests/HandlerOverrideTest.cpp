#include <version>

import stdx;

using namespace stdx::test;

#if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
namespace {
    bool custom_handler_linked = false;
}

/*
 * A strong definition. stdlibx_test_contracts is linked into this binary and
 * carries the weak one, so this TU must win the link without a duplicate-symbol
 * error - that yielding is what this test exists to prove. No contract checks
 * live in this TU (GCC 16.2 miscompiles a TU holding both checks and a handler),
 * so the handler is never fired here; it forwards per the documented pattern.
 */
void handle_contract_violation(const ContractViolation& violation) {
    custom_handler_linked = true;
    stdx::test::on_contract_violation(violation);
}

void test_strong_handler_links_over_the_weak_one() {
    // Reaching this line at all means the link resolved one handler cleanly.
    expect(!custom_handler_linked, "and it has not fired without a violation");
}
#endif

int main(int argc, char* argv[]) {
    #if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
    return run(argc, argv, {
        {"Contracts.strong_handler_links_over_the_weak_one", test_strong_handler_links_over_the_weak_one},
    });
    #else
    System::out.println("Contracts tests skipped (toolchain has no contract support).");
    #endif
}
