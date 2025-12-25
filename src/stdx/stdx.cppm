/**
 * @file stdx.cppm
 * @module stdx
 * @brief Module file for standard library extensions.
 *
 * This file imports all the standard library extension modules.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx;
#else
export module stdlibx;
#endif

export import :core;
export import :arch;
export import :compiler;
export import :crypto;
export import :csv;
export import :ffi;
export import :json;
export import :linq;
export import :math;
export import :net;
export import :os;
export import :process;
export import :sql;
export import :xml;
export import :util;
export import :zip;
