#include <version>

import stdx;

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef __cpp_contracts
void handle_contract_violation(const ContractViolation& v) {
    // ...
}

i32 add(i32 a, i32 b)
    pre (a > 1)
    post (r: r > 1) {
    contract_assert(a != 0);
    return a + b;
}
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_contracts
    i32 result = add(2, 3);
    System::out.println("Result: {}", result);
    #else
    System::out.println("Example disabled (compiler does not support contracts).");
    #endif
}

