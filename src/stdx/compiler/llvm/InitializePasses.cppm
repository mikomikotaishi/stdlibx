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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:compiler.llvm.InitializePasses;
#else
export module stdlibx:compiler.llvm.InitializePasses;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::compiler::llvm {
#else 
export namespace stdlibx::compiler::llvm {
#endif
    using ::llvm::PassRegistry;
    using ::llvm::initializeCore;
}

#endif
