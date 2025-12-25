/**
 * @file core.cppm
 * @module stdx:core
 * @brief Import of core functionality modules.
 *
 * This file imports the modules for core functionality.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:core;
#else
export module stdlibx:core;
#endif

export import :core.ArithmeticException;
export import :core.InvalidOperationException;
export import :core.NotImplementedException;
export import :core.NullPointerException;
export import :core.UnsupportedOperationException;
