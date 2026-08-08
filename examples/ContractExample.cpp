#include <version>

import stdx;

#if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
[[nodiscard]]
i32 addPositive(
    const i32 a,
    const i32 b
)
    pre (a > 0)
    pre (b > 0)
    post (r: r > a)
{
    return a + b;
}
#endif

int main(int argc, char* argv[]) {
    #if defined(__cpp_contracts) && defined(__cpp_lib_contracts)
    const i32 result = addPositive(2, 3);
    contract_assert(result == 5);
    System::out.println("addPositive(2, 3) = {}", result);

    /*
     * Violations are routed to the global contract-violation handler. This
     * program keeps the toolchain default, which reports the violation to
     * stderr and, under the enforce semantic, terminates. Test binaries link
     * stdlibx_test_contracts instead, whose handler turns violations into test
     * failures (ContractsTest shows it in action). A program can also replace
     * the handler for itself by defining ::handle_contract_violation - but do
     * that in a translation unit of its own, following
     * src/stdx/details/contract_violation_handler.cpp: as of GCC 16.2 the
     * experimental contracts runtime miscompiles a replacement handler that
     * lives in the same translation unit as checked code.
     *
     * Run with any argument (e.g. `ContractExample --violate`) to see the
     * report: `a > 0` fails and the default handler terminates the program,
     * which is why the violation is opt-in rather than part of the normal run.
     */
    if (!Environment::args().subspan(1).empty()) {
        const i32 _ = addPositive(0, 3);
    }
    #else
    System::out.println("Example disabled (compiler does not support contracts).");
    #endif
}
