/**
 * @file clang.cppm
 * @module stdx.compiler.clang
 * @brief Module file for Clang operations.
 *
 * This file contains the imports for the Clang operations,
 * located in all <clang/ *.h> files.
 */

module;

#if defined(__clang__) && defined(STDLIBX_EXTENSIONS_COMPILE_COMPILER_CLANG_LIBRARY)
#include <clang/APINotes/APINotesManager.h>
#include <clang/APINotes/APINotesOptions.h>
#include <clang/APINotes/APINotesReader.h>
#include <clang/APINotes/APINotesWriter.h>
#include <clang/APINotes/APINotesYAMLCompiler.h>
#include <clang/APINotes/Types.h>
#endif

export module stdx:compiler.clang;

#include "stdx/compiler/clang/APINotes.inl"
