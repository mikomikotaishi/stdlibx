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

#if 0 // Currently suppressed until CMake support for import std is finalised.
export import std; // Apparently this is needed in some places.
#endif

export import :core;
export import :arch;
export import :collections;
export import :compiler;
export import :crypto;
export import :csv;
export import :execution;
export import :ffi;
export import :fmt;
export import :fs;
export import :future;
export import :io;
export import :iter;
export import :json;
export import :linq;
export import :math;
export import :mem;
export import :meta;
export import :net;
export import :os;
export import :process;
export import :random;
export import :ranges;
export import :sql;
export import :sync;
export import :sys;
export import :text;
export import :thread;
export import :time;
export import :util;
export import :xml;
export import :zip;

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STDX_MODULE_EXPORT_CORE();
#endif
