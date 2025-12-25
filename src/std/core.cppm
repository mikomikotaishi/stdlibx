/**
 * @file core.cppm
 * @module std:core
 * @brief Module file for standard library core operations.
 *
 * This file contains the imports for the core operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core;
#else
export module stdlib:core;
#endif

export import :core.any;
export import :core.array;
export import :core.compare;
export import :core.concepts;
export import :core.contracts;
export import :core.cstddef;
export import :core.exceptions;
export import :core.expected;
export import :core.functional;
export import :core.initializer_list;
export import :core.limits;
export import :core.optional;
export import :core.pair;
export import :core.sequence;
export import :core.span;
export import :core.string;
export import :core.string_view;
export import :core.system_error;
export import :core.tags;
export import :core.tuple;
export import :core.variant;
