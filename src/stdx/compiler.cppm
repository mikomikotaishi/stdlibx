/**
 * @file compiler.cppm
 * @module stdx:compiler
 * @brief Module file for compiler operations.
 *
 * This file contains the imports for the compiler operations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:compiler;
#else
export module stdlibx:compiler;
#endif

export import :compiler.clang;
export import :compiler.llvm;
