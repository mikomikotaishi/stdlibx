/**
 * @file math.cppm
 * @module std:math
 * @brief Module file for standard library mathematical operations.
 *
 * This file contains the imports for the mathematical operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math;
#else
export module stdlib:math;
#endif

export import :math.algorithm;
export import :math.bit;
export import :math.cmath;
export import :math.complex;
export import :math.cstddef;
export import :math.numbers;
export import :math.numeric;
export import :math.random;
export import :math.ratio;
