/**
 * @file logging.cppm
 * @module stdx:util.logging
 * @brief Module file for logging operations.
 *
 * This file contains the implementation of the logging operations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util.logging;
#else
export module stdlibx:util.logging;
#endif

export import :util.logging.Level;
export import :util.logging.Logger;
export import :util.logging.LoggerFactory;
export import :util.logging.Sinks;
