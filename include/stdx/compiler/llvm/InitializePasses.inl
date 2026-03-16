#pragma once

export namespace stdx::compiler::llvm {
#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_LLVM_LIBRARY
    using ::llvm::PassRegistry;
    using ::llvm::initializeCore;
#endif
}
