/**
 * @file InitializePasses.cppm
 * @module stdx:compiler.llvm.InitializePasses
 * @brief LLVM header file
 */

module;

#if defined(__clang__) && defined(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
#include <llvm/InitializePasses.h>
#include <llvm/PassRegistry.h>
#endif

export module stdx:compiler.llvm.InitializePasses;

export namespace stdx::compiler::llvm {
#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY
    using ::llvm::PassRegistry;
    using ::llvm::initializeCore;
#endif
}
