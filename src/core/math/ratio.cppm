/**
 * @file ratio.cppm
 * @module core:math.ratio
 * @brief Module file for standard library ratio operations.
 *
 * This file contains the implementation of the ratio operations in the standard library.
 */

module;

#include <ratio>

export module core:math.ratio;

/**
 * @namespace core::math::ratio
 * @brief Wrapper namespace for standard library ratio operations.
 */
export namespace core::math::inline ratio {
    template <std::intmax_t Num, std::intmax_t Denom = 1>
    using Ratio = std::ratio<Num, Denom>;

    template <typename R1, typename R2>
    using RatioAdd = std::ratio_add<R1, R2>;

    template <typename R1, typename R2>
    using RatioSubtract = std::ratio_subtract<R1, R2>;

    template <typename R1, typename R2>
    using RatioMultiply = std::ratio_multiply<R1, R2>;

    template <typename R1, typename R2>
    using RatioDivide = std::ratio_divide<R1, R2>;

    template <typename R1, typename R2>
    using RatioEqual = std::ratio_equal<R1, R2>;

    template <typename R1, typename R2>
    using RatioNotEqual = std::ratio_not_equal<R1, R2>;

    template <typename R1, typename R2>
    using RatioLess = std::ratio_less<R1, R2>;

    template <typename R1, typename R2>
    using RatioLessEqual = std::ratio_less_equal<R1, R2>;

    template <typename R1, typename R2>
    using RatioGreater = std::ratio_greater<R1, R2>;

    template <typename R1, typename R2>
    using RatioGreaterEqual = std::ratio_greater_equal<R1, R2>;

    #if __INTMAX_WIDTH__ >= 96
    #if __INTMAX_WIDTH__ >= 128
    using QUECTO = std::quecto;
    #endif 
    using RONTO = std::ronto;
    using YOCTO = std::yocto;
    using ZEPTO = std::zepto;
    #endif
    using ATTO = std::atto;
    using FEMTO =std::femto;
    using PICO = std::pico;
    using NANO = std::nano;
    using MICRO = std::micro;
    using MILLI = std::milli;
    using CENTI = std::centi;
    using DECI = std::deci;
    using DECA = std::deca;
    using HECTO = std::hecto;
    using KILO = std::kilo;
    using MEGA = std::mega;
    using GIGA = std::giga;
    using TERA = std::tera;
    using PETA = std::peta;
    using EXA = std::exa;
    #if __INTMAX_WIDTH__ >= 96
    using ZETTA = std::zetta;
    using YOTTA = std::yotta;
    using RONNA = std::ronna;
    #if __INTMAX_WIDTH__ >= 128
    using QUETTA = std::quetta;
    #endif 
    #endif
}
