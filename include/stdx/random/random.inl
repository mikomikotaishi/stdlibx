#pragma once

/**
 * @internal
 * @namespace _detail::core
 * @brief Wrapper namespace for standard library random number operations.
 * Detail namespace - not to be exported for external use
 */
namespace _detail::stdx::random {
    constexpr auto RandMax = RAND_MAX;
}

#undef RAND_MAX

/**
 * @namespace stdx::random
 * @brief Wrapper namespace for standard library random number operations.
 */
export namespace stdx::random {
    template <typename G>
    concept UniformRandomBitGenerator = std::uniform_random_bit_generator<G>;

    // Engines
    template <typename UIntType, UIntType a, UIntType c, UIntType m>
    using LinearCongruentialEngine = std::linear_congruential_engine<UIntType, a, c, m>;

    template <
        typename UIntType, std::size_t w, std::size_t n, std::size_t m, std::size_t r,
        UIntType a, std::size_t u, std::size_t d, std::size_t s,
        UIntType b, std::size_t t, std::size_t c, std::size_t l, 
        UIntType f
    >
    using MersenneTwisterEngine = std::mersenne_twister_engine<UIntType, w, n, m, r, a, u, d, s, b, t, c, l, f>;

    template <typename UIntType, std::size_t w, std::size_t s, std::size_t r>
    using SubtractWithCarryEngine = std::subtract_with_carry_engine<UIntType, w, s, r>;

    // Adaptors
    template <typename RandomNumberEngine, std::size_t p, std::size_t r>
    using DiscardBlockEngine = std::discard_block_engine<RandomNumberEngine, p, r>;

    template <typename RandomNumberEngine, std::size_t w, typename UIntType>
    using IndependentBitsEngine = std::independent_bits_engine<RandomNumberEngine, w, UIntType>;

    template <typename RandomNumberEngine, std::size_t k>
    using ShuffleOrderEngine = std::shuffle_order_engine<RandomNumberEngine, k>;

    // Predefined Generators
    using MinimalStandardRandom0 = std::minstd_rand0;
    using MinimalStandardRandom = std::minstd_rand;
    using MersenneTwister = std::mt19937;
    using MersenneTwister64 = std::mt19937_64;
    using Ranlux24BaseEngine = std::ranlux24_base;
    using Ranlux48BaseEngine = std::ranlux48_base;
    using Ranlux24Engine = std::ranlux24;
    using Ranlux48Engine = std::ranlux48;
    using KnuthBGenerator = std::knuth_b;

    // Non-deterministic random numbers
    using RandomDevice = std::random_device;

    // Distributions
    template <typename IntType = int>
    using UniformIntegerDistribution = std::uniform_int_distribution<IntType>;

    template <typename RealType = double>
    using UniformRealDistribution = std::uniform_real_distribution<RealType>;

    using BernoulliDistribution = std::bernoulli_distribution;

    template <typename IntType = int>
    using BinomialDistribution = std::binomial_distribution<IntType>;

    template <typename IntType = int>
    using NegativeBinomialDistribution = std::negative_binomial_distribution<IntType>;

    template <typename IntType = int>
    using GeometricDistribution = std::geometric_distribution<IntType>;

    template <typename IntType = int>
    using PoissonDistribution = std::poisson_distribution<IntType>;

    template <typename RealType = double>
    using ExponentialDistribution = std::exponential_distribution<RealType>;

    template <typename RealType = double>
    using GammaDistribution = std::gamma_distribution<RealType>;

    template <typename RealType = double>
    using WeibullDistribution = std::weibull_distribution<RealType>;

    template <typename RealType = double>
    using ExtremeValueDistribution = std::extreme_value_distribution<RealType>;

    template <typename RealType = double>
    using NormalDistribution = std::normal_distribution<RealType>;

    template <typename RealType = double>
    using LogNormalDistribution = std::lognormal_distribution<RealType>;

    template <typename RealType = double>
    using ChiSquaredDistribution = std::chi_squared_distribution<RealType>;

    template <typename RealType = double>
    using CauchyDistribution = std::cauchy_distribution<RealType>;

    template <typename RealType = double>
    using FisherFDistribution = std::fisher_f_distribution<RealType>;

    template <typename RealType = double>
    using StudentTDistribution = std::student_t_distribution<RealType>;

    template <typename IntType = int>
    using DiscreteDistribution = std::discrete_distribution<IntType>;

    template <typename RealType = double>
    using PiecewiseConstantDistribution = std::piecewise_constant_distribution<RealType>;

    template <typename RealType = double>
    using PiecewiseLinearDistribution = std::piecewise_linear_distribution<RealType>;

    // Seed sequence
    using SeedSequence = std::seed_seq;
    
    // Functions
    using std::generate_canonical;

    using std::rand;
    using std::srand;

    constexpr auto RAND_MAX = ::_detail::stdx::random::RandMax;
}
