/**
 * @file llvm.cppm
 * @module stdx:compiler.llvm
 * @brief Module file for LLVM operations.
 *
 * This file contains the imports for the LLVM operations,
 * located in all <llvm/ *.h> files.
 */

module;

#if defined(__clang__) && defined(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LLVM_LIBRARY)
#include <llvm/InitializePasses.h>
#include <llvm/LinkAllIR.h>
#include <llvm/LinkAllPasses.h>
#include <llvm/Pass.h>
#include <llvm/PassInfo.h>
#include <llvm/PassRegistry.h>
#endif

export module stdx:compiler.llvm;

#include "stdx/compiler/llvm/InitializePasses.inl"
#include "stdx/compiler/llvm/Pass.inl"
