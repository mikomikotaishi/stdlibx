#pragma once

using stdx::iter::Permutable;
using stdx::iter::RandomAccessIterator;
using stdx::iter::SentinelFor;
using stdx::meta::InvokeResultType;
using stdx::random::MersenneTwister;
using stdx::random::NormalDistribution;
using stdx::random::RandomDevice;
using stdx::random::SeedSequence;
using stdx::random::UniformIntegerDistribution;
using stdx::random::UniformRandomBitGenerator;
using stdx::random::UniformRealDistribution;
using stdx::ranges::BorrowedIteratorType;
using stdx::ranges::OutputRange;
using stdx::ranges::RangeIterator;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @brief A class for generating random numbers.
     * 
     * @tparam Engine The random number engine to use.
     */
    template <UniformRandomBitGenerator Engine = MersenneTwister>
    class [[nodiscard]] Random {
    private:
        Engine engine;
    public:
        using Seed = typename Engine::result_type;

        explicit Random(Seed seed = RandomDevice{}()) noexcept:
            engine{seed} {}

        explicit Random(SeedSequence seq) noexcept:
            engine{seq} {}

        ~Random() = default;

        template <Numeric T>
            requires (!SameAs<T, bool>)
        [[nodiscard]]
        T next(T min, T max) throws (InvalidArgumentException) {
            if (min >= max) {
                throw InvalidArgumentException("min must be less than max");
            }

            if constexpr (Integral<T>) {
                UniformIntegerDistribution<T> dist(min, max - 1);
                return dist(engine);
            } else if constexpr (FloatingPoint<T>) {
                UniformRealDistribution<T> dist(min, max);
                return dist(engine);
            }
        }

        template <Integral Int>
        [[nodiscard]]
        Int next(Int max = NumericLimits<Int>::max()) throws (InvalidArgumentException) {
            return next<Int>(0, max);
        }

        template <FloatingPoint Flt>
        [[nodiscard]]
        Flt next(Flt max = NumericLimits<Flt>::max()) throws (InvalidArgumentException) {
            return next<Flt>(0.0, max);
        }

        template <FloatingPoint Flt = f64>
        [[nodiscard]]
        Flt next_unit() noexcept {
            if constexpr (SameAs<Flt, f32>) {
                return (engine() >> 8) * (1.0f / (1u << 24));
            } else if constexpr (SameAs<Flt, f64>) {
                return (((engine() >> 5) << 26) + (engine() >> 6)) * (1.0 / (1ULL << 53));
            } else {
                UniformRealDistribution<Flt> dist(0.0, 1.0);
                return dist(engine);
            }
        }

        [[nodiscard]]
        bool chance(f64 probability = 0.5) noexcept {
            // Must static_cast 0.0 and 1.0 to f64 due to GCC treating _Float64 and double differently
            return next_unit() < Math::clamp(probability, static_cast<f64>(0.0), static_cast<f64>(1.0));
        }

        template <FloatingPoint Flt = f64>
        [[nodiscard]]
        Flt next_gaussian(Flt mean = 0.0, Flt stddev = 1.0) noexcept {
            static thread_local NormalDistribution<Flt> dist(0.0, 1.0);
            return dist(engine) * stddev + mean;
        }

        template <RandomAccessIterator Iter, SentinelFor<Iter> S>
            requires Permutable<Iter>
        Iter shuffle(Iter first, S last) {
            return stdx::ranges::shuffle(first, last, engine);
        }

        template <typename Ran>
            requires Permutable<RangeIterator<Ran>>
        BorrowedIteratorType<Ran> shuffle(Ran&& r) {
            return stdx::ranges::shuffle(stdx::util::forward<Ran>(r), engine);
        }

        #ifdef __cpp_lib_ranges_generate_random
        template <typename Ran>
            requires OutputRange<Ran, InvokeResultType<Engine&>>
        constexpr BorrowedIteratorType<Ran> generate_random(Ran&& r) {
            return stdx::ranges::generate_random(stdx::util::forward<Ran>(r), engine);
        }
        #endif

        void seed(Seed s = RandomDevice{}()) noexcept {
            engine.seed(s);
        }

        void seed(SeedSequence seq) noexcept {
            engine.seed(seq);
        }

        [[nodiscard]]
        Engine& state() noexcept {
            return engine;
        }

        [[nodiscard]]
        const Engine& state() const noexcept {
            return engine;
        }
    };
}
