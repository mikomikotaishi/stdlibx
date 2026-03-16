/**
 * @file stdx.cppm
 * @module stdx
 * @brief Module file for standard library extensions.
 *
 * This file imports all the standard library extension modules.
 */

module;

export module stdx;

export import :main;
export import :arch;
export import :compiler;
export import :crypto;
export import :csv;
export import :ffi;
export import :json;
export import :lua;
export import :math.gmp;
export import :os;
export import :sql;
export import :xml;
export import :zip;

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
export using namespace stdx::core;
#endif

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_LITERALS)
export using namespace stdx::literals;
#endif
