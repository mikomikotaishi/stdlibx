#pragma once

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
    using Quecto = std::quecto;
    #endif 
    using Ronto = std::ronto;
    using Yocto = std::yocto;
    using Zepto = std::zepto;
    #endif
    using Atto = std::atto;
    using Femto =std::femto;
    using Pico = std::pico;
    using Nano = std::nano;
    using Micro = std::micro;
    using Milli = std::milli;
    using Centi = std::centi;
    using Deci = std::deci;
    using Deca = std::deca;
    using Hecto = std::hecto;
    using Kilo = std::kilo;
    using Mega = std::mega;
    using Giga = std::giga;
    using Tera = std::tera;
    using Peta = std::peta;
    using Exa = std::exa;
    #if __INTMAX_WIDTH__ >= 96
    using Zetta = std::zetta;
    using Yotta = std::yotta;
    using Ronna = std::ronna;
    #if __INTMAX_WIDTH__ >= 128
    using Quetta = std::quetta;
    #endif 
    #endif
}
