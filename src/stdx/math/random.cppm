/**
 * @file random.cppm
 * @module std:math.random
 * @brief Module file for standard library random number operations.
 *
 * This file contains the implementation of the random number operations in the standard library.
 */

module;

export module stdx:math.random;

import :random;

/**
 * @namespace std::math::random
 * @brief Wrapper namespace for standard library random number operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::math::inline random {
#else 
export namespace stdlib::math::inline random {
#endif
    using stdx::random::UniformRandomBitGenerator;
    using stdx::random::LinearCongruentialEngine;
    using stdx::random::MersenneTwisterEngine;
    using stdx::random::SubtractWithCarryEngine;
    using stdx::random::DiscardBlockEngine;
    using stdx::random::ShuffleOrderEngine;
    using stdx::random::MinimalStandardRandom0;
    using stdx::random::MinimalStandardRandom;
    using stdx::random::MersenneTwister;
    using stdx::random::MersenneTwister64;
    using stdx::random::Ranlux24BaseEngine;
    using stdx::random::Ranlux48BaseEngine;
    using stdx::random::Ranlux24Engine;
    using stdx::random::Ranlux48Engine;
    using stdx::random::KnuthBGenerator;
    using stdx::random::RandomDevice;
    using stdx::random::UniformIntegerDistribution;
    using stdx::random::UniformRealDistribution;
    using stdx::random::BernoulliDistribution;
    using stdx::random::BinomialDistribution;
    using stdx::random::NegativeBinomialDistribution;
    using stdx::random::GeometricDistribution;
    using stdx::random::PoissonDistribution;
    using stdx::random::ExponentialDistribution;
    using stdx::random::GammaDistribution;
    using stdx::random::WeibullDistribution;
    using stdx::random::ExtremeValueDistribution;
    using stdx::random::NormalDistribution;
    using stdx::random::LogNormalDistribution;
    using stdx::random::ChiSquaredDistribution;
    using stdx::random::CauchyDistribution;
    using stdx::random::FisherFDistribution;
    using stdx::random::StudentTDistribution;
    using stdx::random::DiscreteDistribution;
    using stdx::random::PiecewiseLinearDistribution;
    using stdx::random::SeedSequence;
    
    using stdx::random::generate_canonical;
}
