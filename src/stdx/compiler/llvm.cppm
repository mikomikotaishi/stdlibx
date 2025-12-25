/**
 * @file llvm.cppm
 * @module stdx:compiler.llvm
 * @brief Module file for LLVM operations.
 *
 * This file contains the imports for the LLVM operations,
 * located in all <llvm/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:compiler.llvm;
#else
export module stdlibx:compiler.llvm;
#endif

export import :compiler.llvm.InitializePasses;
export import :compiler.llvm.Pass;
