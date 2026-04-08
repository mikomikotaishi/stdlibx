#include <version>

import stdx;

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef __cpp_contracts
i32 add(i32 a, i32 b)
    pre (a > 1)
    post (r: r > 1) {
    return a + b;
}
#endif

int main() {
    #ifdef __cpp_contracts

    #else
    System::out.println("Test disabled.");
    #endif
}

