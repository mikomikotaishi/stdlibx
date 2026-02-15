/**
 * @file numbers.cppm
 * @module stdx:math.numbers
 * @brief Module file for standard library mathematical constants.
 *
 * This file contains the implementation of the mathematical constants in the standard library.
 */

module;

export module stdx:math.numbers;

import core;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    #if __has_include(<numbers>)
    using core::math::Numbers;
    #endif
}
