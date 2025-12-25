/**
 * @file math.cppm
 * @module stdx:math
 * @brief Import of mathematics modules.
 *
 * This file imports the modules for the mathematics library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:math;
#else
export module stdlibx:math;
#endif

export import :math.BigDecimal;
export import :math.BigInteger;
