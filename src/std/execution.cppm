/**
 * @file execution.cppm
 * @module std:execution
 * @brief Module file for standard library execution operations.
 *
 * This file contains the imports for the execution policies for standard algorithms in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:execution;
#else
export module stdlib:execution;
#endif

export import :execution.execution;
