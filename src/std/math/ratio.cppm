/**
 * @file ratio.cppm
 * @module std:math.ratio
 * @brief Module file for standard library ratio operations.
 *
 * This file contains the implementation of the ratio operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.ratio;
#else
export module stdlib:math.ratio;
#endif

import core;

/**
 * @namespace std::math::ratio
 * @brief Wrapper namespace for standard library mathematical operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::math::inline ratio {
#else 
export namespace stdlib::math::inline ratio {
#endif
    using core::math::ratio::Ratio;
    using core::math::ratio::RatioAdd;
    using core::math::ratio::RatioSubtract;
    using core::math::ratio::RatioMultiply;
    using core::math::ratio::RatioDivide;
    using core::math::ratio::RatioEqual;
    using core::math::ratio::RatioNotEqual;
    using core::math::ratio::RatioLess;
    using core::math::ratio::RatioLessEqual;
    using core::math::ratio::RatioGreater;
    using core::math::ratio::RatioGreaterEqual;
    
    #if __INTMAX_WIDTH__ >= 96
    #if __INTMAX_WIDTH__ >= 128
    using core::math::ratio::QUECTO;
    #endif 
    using core::math::ratio::RONTO;
    using core::math::ratio::YOCTO;
    using core::math::ratio::ZEPTO;
    #endif
    using core::math::ratio::ATTO;
    using core::math::ratio::FEMTO;
    using core::math::ratio::PICO;
    using core::math::ratio::NANO;
    using core::math::ratio::MICRO;
    using core::math::ratio::MILLI;
    using core::math::ratio::CENTI;
    using core::math::ratio::DECI;
    using core::math::ratio::DECA;
    using core::math::ratio::HECTO;
    using core::math::ratio::KILO;
    using core::math::ratio::MEGA;
    using core::math::ratio::GIGA;
    using core::math::ratio::TERA;
    using core::math::ratio::PETA;
    using core::math::ratio::EXA;
    #if __INTMAX_WIDTH__ >= 96
    using core::math::ratio::ZETTA;
    using core::math::ratio::YOTTA;
    using core::math::ratio::RONNA;
    #if __INTMAX_WIDTH__ >= 128
    using core::math::ratio::QUETTA;
    #endif 
    #endif
}
