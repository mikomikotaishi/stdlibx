#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @class SecureRandom
     * @brief Cryptographically secure random number and byte generator.
     *
     * All output is produced by libsodium's {@code randombytes_*} family, which
     * sources entropy from the operating system's secure random facility.
     */
    class SecureRandom {
    public:
        SecureRandom() = default;

        /**
         * @brief Returns a cryptographically secure random value in {@code [min, max)}.
         * @param min Inclusive lower bound.
         * @param max Exclusive upper bound.
         * @return A random value in {@code [min, max)}.
         * @throws InvalidArgumentException if {@code min >= max}.
         *
         * For integer types up to 32 bits, {@code randombytes_uniform()} is used,
         * which internally applies rejection sampling to eliminate modulo bias.
         * For wider integer types and floating-point types, {@code randombytes_buf()}
         * is used directly with appropriate scaling.
         */
        template <Numeric T>
            requires (!SameAs<T, bool>)
        [[nodiscard]]
        THROWS(InvalidArgumentException)
        T next(T min, T max) {
            ensure_sodium_initialized();
            if (min >= max) {
                throw InvalidArgumentException("min must be less than max");
            }
            if constexpr (Integral<T>) {
                if constexpr (sizeof(T) <= 4) {
                    // randombytes_uniform applies rejection sampling internally
                    const u32 range = static_cast<u32>(max - min);
                    return static_cast<T>(randombytes_uniform(range)) + min;
                } else {
                    // 64-bit rejection sampling.
                    //
                    // Threshold = 2^64 mod range, computed as (-range) % range.
                    // Values in [threshold, 2^64) are accepted; their count is
                    // floor(2^64/range)*range, which is exactly divisible by range,
                    // guaranteeing an unbiased result.
                    //
                    // Crucially, (-range) % range == 0 when range is a power of 2
                    // or range == 1, so no samples are ever rejected in those cases.
                    // The old formula (UINT64_MAX - UINT64_MAX%range)+1 overflows to
                    // 0 for range==1, causing an infinite loop.
                    const u64 range = static_cast<u64>(max) - static_cast<u64>(min);
                    const u64 threshold = (-range) % range;
                    u64 raw;
                    do {
                        randombytes_buf(&raw, sizeof(raw));
                    } while (raw < threshold);
                    // Reconstruct the signed result via unsigned offset arithmetic,
                    // which handles negative min and cross-zero ranges correctly.
                    return static_cast<T>(static_cast<u64>(min) + (raw % range));
                }
            } else if constexpr (FloatingPoint<T>) {
                u64 bits;
                randombytes_buf(&bits, sizeof(bits));
                // Convert 53 random bits to a value in [0, 1)
                const f64 unit = static_cast<f64>(bits >> 11) * (1.0 / static_cast<f64>(1ull << 53));
                return static_cast<T>(unit) * (max - min) + min;
            }
        }

        /**
         * @brief Returns a cryptographically secure random integer in {@code [0, max)}.
         * @param max Exclusive upper bound.
         * @throws InvalidArgumentException if {@code max <= 0}.
         */
        template <Integral Int>
            requires (!SameAs<Int, bool>)
        [[nodiscard]]
        THROWS(InvalidArgumentException)
        Int next(Int max = NumericLimits<Int>::max()) {
            return next<Int>(Int{0}, max);
        }

        /**
         * @brief Returns a cryptographically secure random floating-point value in {@code [0, max)}.
         * @param max Exclusive upper bound.
         * @throws InvalidArgumentException if {@code max <= 0}.
         */
        template <FloatingPoint Flt>
        [[nodiscard]]
        THROWS(InvalidArgumentException)
        Flt next(Flt max = NumericLimits<Flt>::max()) {
            return next<Flt>(Flt{0}, max);
        }

        /**
         * @brief Returns a cryptographically secure random value in {@code [0, 1)}.
         *
         * Generates 53 bits of OS entropy and scales to the double-precision unit interval,
         * matching the bit-width used by {@code Random::next_unit<f64>()}.
         *
         * @note {@code sodium_init()} is not called here because {@code randombytes_buf}
         * initializes the OS entropy source itself, and this method is {@code noexcept}.
         */
        template <FloatingPoint Flt = f64>
        [[nodiscard]]
        Flt next_unit() noexcept {
            u64 bits;
            randombytes_buf(&bits, sizeof(bits));
            return static_cast<Flt>(static_cast<f64>(bits >> 11) * (1.0 / static_cast<f64>(1ull << 53)));
        }

        /**
         * @brief Returns {@code true} with cryptographically secure probability {@code probability}.
         * @param probability Clamped to {@code [0, 1]}.
         */
        [[nodiscard]]
        bool chance(f64 probability = 0.5) noexcept {
            // Must static_cast 0.0 and 1.0 to f64 due to GCC treating _Float64 and double differently
            return next_unit() < Math::clamp(probability, static_cast<f64>(0.0), static_cast<f64>(1.0));
        }

        /**
         * @brief Fills {@code out} with cryptographically secure random bytes in-place.
         * @param out The buffer to fill.
         */
        void next_bytes(Span<u8> out) {
            ensure_sodium_initialized();
            if (!out.empty()) {
                randombytes_buf(out.data(), out.size());
            }
        }

        /**
         * @brief Returns a new buffer of {@code size} cryptographically secure random bytes.
         * @param size The number of random bytes to generate.
         * @return A {@code ByteBuffer} of length {@code size} filled with random data.
         */
        [[nodiscard]]
        ByteBuffer next_bytes(usize size) {
            ByteBuffer out(size, 0);
            next_bytes(Span<u8>(out.data(), out.size()));
            return out;
        }
    };
}
#endif
