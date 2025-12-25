/**
 * @file linq.cppm
 * @module stdx:linq
 * @brief Import of LINQ modules.
 *
 * This file imports the modules for the LINQ-like query library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:linq;
#else
export module stdlibx:linq;
#endif

export import :linq.Query;
