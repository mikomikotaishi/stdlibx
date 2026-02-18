#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    using NumericLimits = std::numeric_limits<T>;

    using FloatRoundStyle = std::float_round_style;
    using FloatDenormStyle = std::float_denorm_style;
}
