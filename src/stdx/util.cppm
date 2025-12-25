/**
 * @file util.cppm
 * @module stdx:util
 * @brief Import of utility modules.
 *
 * This file imports the modules for the utility library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util;
#else
export module stdlibx:util;
#endif

export import :util.ArgumentParser;
export import :util.ParsedArguments;
export import :util.logging;
