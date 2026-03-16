#pragma once

using stdx::math::Complex;
using stdx::random::MersenneTwister;
using stdx::random::RandomDevice;
using stdx::random::UniformRealDistribution;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #include "Math.inl"
}
