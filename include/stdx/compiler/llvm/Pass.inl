#pragma once

export namespace stdx::compiler::llvm {
#ifdef STDLIBX_EXTENSIONS_COMPILE_COMPILER_LLVM_LIBRARY
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
