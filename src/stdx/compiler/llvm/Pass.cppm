/**
 * @file Pass.cppm
 * @module stdx:compiler.llvm.Pass
 * @brief LLVM header file
 */

module;

#if defined(__clang__) && defined(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
#include <llvm/Pass.h>
#endif

export module stdx:compiler.llvm.Pass;

export namespace stdx::compiler::llvm {
#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY
    using ::llvm::AnalysisID;
    using ::llvm::PassManagerType;
    using ::llvm::PassKind;
    using ::llvm::ThinOrFullLTOPhase;
    using ::llvm::Pass;
    using ::llvm::ModulePass;
    using ::llvm::ImmutablePass;
    using ::llvm::FunctionPass;
    using ::llvm::TimePassesIsEnabled;
    using ::llvm::TimePassesPerRun;
#endif
}
