/**
 * @file stdx.cppm
 * @module stdx
 * @brief Module file for standard library extensions.
 *
 * This file imports all the standard library extension modules.
 */

module;

#include "Macros.hpp"

export module stdx;

export import std; // Apparently this is needed in some places.

export import :main;
export import :arch;
export import :compiler;
export import :crypto;
export import :csv;
export import :ffi;
export import :fs.glob;
export import :json;
export import :linq;
export import :lua;
export import :os;
export import :process;
export import :sql;
export import :util;
export import :util.logging;
export import :xml;
export import :zip;

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STDX_MODULE_EXPORT_CORE();
#endif
