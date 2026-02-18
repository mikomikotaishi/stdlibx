/**
 * @file core.cppm
 * @module core
 * @brief Module file for standard library core operations.
 *
 * This file contains the imports for the core operations in the standard library.
 */

module;

export module core;

import std;

export import :main;
export import :constants;

#if defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
