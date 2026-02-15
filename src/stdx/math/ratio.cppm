/**
 * @file ratio.cppm
 * @module stdx:math.ratio
 * @brief Module file for standard library ratio operations.
 *
 * This file contains the implementation of the ratio operations in the standard library.
 */

module;

export module stdx:math.ratio;

import core;

/**
 * @namespace stdx::math::ratio
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math::inline ratio {
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
    using core::math::ratio::Quecto;
    #endif 
    using core::math::ratio::Ronna;
    using core::math::ratio::Yocto;
    using core::math::ratio::Zepto;
    #endif
    using core::math::ratio::Atto;
    using core::math::ratio::Femto;
    using core::math::ratio::Pico;
    using core::math::ratio::Nano;
    using core::math::ratio::Micro;
    using core::math::ratio::Milli;
    using core::math::ratio::Centi;
    using core::math::ratio::Deci;
    using core::math::ratio::Deca;
    using core::math::ratio::Hecto;
    using core::math::ratio::Kilo;
    using core::math::ratio::Mega;
    using core::math::ratio::Giga;
    using core::math::ratio::Tera;
    using core::math::ratio::Peta;
    using core::math::ratio::Exa;
    #if __INTMAX_WIDTH__ >= 96
    using core::math::ratio::Zetta;
    using core::math::ratio::Yotta;
    using core::math::ratio::Ronna;
    #if __INTMAX_WIDTH__ >= 128
    using core::math::ratio::Quetta;
    #endif 
    #endif
}
