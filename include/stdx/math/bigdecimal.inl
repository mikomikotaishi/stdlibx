#pragma once

/**
 * @namespace stdx::math
 * @brief Standard library mathematical operations.
 */
export namespace stdx::math {
    /**
     * @enum RoundingMode
     * @brief Rounding behaviour for operations capable of discarding precision.
     */
    enum class RoundingMode: u8 {
        UP, ///< Round away from zero.
        DOWN, ///< Round towards zero.
        CEILING, ///< Round towards positive infinity.
        FLOOR, ///< Round towards negative infinity.
        HALF_UP, ///< Round to nearest neighbour, ties away from zero.
        HALF_DOWN, ///< Round to nearest neighbour, ties towards zero.
        HALF_EVEN, ///< Round to nearest neighbour, ties to the even neighbour.
        UNNECESSARY, ///< Assert exactness; throw if rounding would be required.
    };

    /**
     * @class MathContext
     * @brief Settings describing rules for numerical operators: a precision and
     * a rounding mode.
     */
    class [[nodiscard]] MathContext final {
    private:
        u32 digits = 0; ///< The number of significant digits, 0 for unlimited.
        RoundingMode rounding = RoundingMode::HALF_UP; ///< The rounding mode.
    public:
        /**
            * @brief Constructs the unlimited-precision context.
            */
        constexpr MathContext() noexcept = default;

        /**
            * @brief Constructs a context with the given precision and rounding mode.
            * @param precision_value The number of significant digits (0 for unlimited).
            * @param mode The rounding mode to apply.
            */
        constexpr explicit MathContext(
            u32 precision_value,
            RoundingMode mode = RoundingMode::HALF_UP
        ) noexcept:
            digits{precision_value}, rounding{mode} {}

        static const MathContext UNLIMITED; ///< Unlimited precision arithmetic.
        static const MathContext DECIMAL32; ///< IEEE 754R Decimal32: 7 digits, HALF_EVEN.
        static const MathContext DECIMAL64; ///< IEEE 754R Decimal64: 16 digits, HALF_EVEN.
        static const MathContext DECIMAL128; ///< IEEE 754R Decimal128: 34 digits, HALF_EVEN.

        /**
            * @brief Returns the number of significant digits (0 means unlimited).
            * @return The number of significant digits, or 0 for unlimited.
            */
        [[nodiscard]]
        constexpr u32 precision() const noexcept {
            return digits;
        }

        /**
            * @brief Returns the rounding mode.
            * @return The rounding mode.
            */
        [[nodiscard]]
        constexpr RoundingMode rounding_mode() const noexcept {
            return rounding;
        }

        [[nodiscard]]
        constexpr bool operator==(const MathContext&) const noexcept = default;
    };

    inline constexpr MathContext MathContext::UNLIMITED{0, RoundingMode::HALF_UP};
    inline constexpr MathContext MathContext::DECIMAL32{7, RoundingMode::HALF_EVEN};
    inline constexpr MathContext MathContext::DECIMAL64{16, RoundingMode::HALF_EVEN};
    inline constexpr MathContext MathContext::DECIMAL128{34, RoundingMode::HALF_EVEN};

    /**
     * @class BigDecimal
     * @brief Immutable arbitrary-precision signed decimal number.
     *
     * The value is represented as an arbitrary-precision
     * unscaled BigInteger value and a 32-bit scale,
     * the represented number being (unscaled_value * 10^-scale).
     */
    class [[nodiscard]] BigDecimal final {
    private:
        BigInteger int_val; ///< The unscaled value.
        i32 scale_val = 0; ///< The scale; the value is int_val * 10^-scale_val.

        /**
         * @brief Returns a hash code.
         * @return The hash code of this BigDecimal.
         */
        [[nodiscard]]
        usize hash_code() const noexcept {
            return 31uz * Hash<BigInteger>{}(int_val) + scale_val;
        }

        friend struct stdx::core::Hash<BigDecimal>;

        [[nodiscard]]
        static BigInteger pow_ten(i64 exponent) throws (ArithmeticException) {
            if (exponent < 0 || exponent > Integer::MAX_VALUE) {
                throw ArithmeticException("Scale overflow.");
            }
            return BigInteger::TEN.pow(static_cast<i32>(exponent));
        }

        [[nodiscard]]
        static i32 check_scale(i64 value) throws (ArithmeticException) {
            if (value > Integer::MAX_VALUE) {
                throw ArithmeticException("Scale overflow.");
            }
            if (value < Integer::MIN_VALUE) {
                throw ArithmeticException("Scale underflow.");
            }
            return static_cast<i32>(value);
        }

        [[nodiscard]]
        static i32 saturate_scale(i64 value) noexcept {
            if (value > Integer::MAX_VALUE) {
                return Integer::MAX_VALUE;
            }
            if (value < Integer::MIN_VALUE) {
                return Integer::MIN_VALUE;
            }
            return static_cast<i32>(value);
        }

        /**
         * @brief Returns the unscaled value rescaled to the given (larger or equal) scale.
         * @param target_scale The target scale, at least this BigDecimal's scale.
         * @return The unscaled value rescaled to target_scale.
         */
        [[nodiscard]]
        BigInteger aligned_unscaled(i32 target_scale) const {
            return target_scale == scale_val
                ? int_val
                : int_val * pow_ten(static_cast<i64>(target_scale) - scale_val);
        }

        /**
         * @brief Compares two unscaled values, returning a weak ordering of their numeric values.
         * @param lhs The left-hand value.
         * @param rhs The right-hand value.
         * @return The weak ordering of lhs and rhs.
         */
        [[nodiscard]]
        static WeakOrdering compare(const BigInteger& lhs, const BigInteger& rhs) noexcept {
            if (lhs < rhs) {
                return WeakOrdering::LESS;
            }
            return lhs > rhs ? WeakOrdering::GREATER : WeakOrdering::EQUIVALENT;
        }

        /**
         * @brief Divides numerator by denominator, rounding the quotient per the mode.
         * @param numerator The dividend.
         * @param denominator The divisor.
         * @param rounding_mode The rounding mode to apply.
         * @return The rounded quotient.
         * @throws ArithmeticException If rounding_mode is UNNECESSARY but rounding is required.
         */
        [[nodiscard]]
        static BigInteger divide_rounded(
            const BigInteger& numerator,
            const BigInteger& denominator,
            RoundingMode rounding_mode
        ) throws (ArithmeticException) {
            auto [quotient, rem] = numerator.divide_and_remainder(denominator);
            if (rem.signum() == 0) {
                return quotient;
            }
            const i32 result_signum = numerator.signum() * denominator.signum();
            bool increment = false;
            switch (rounding_mode) {
                case RoundingMode::UP:
                    increment = true;
                    break;
                case RoundingMode::DOWN:
                    break;
                case RoundingMode::CEILING:
                    increment = result_signum > 0;
                    break;
                case RoundingMode::FLOOR:
                    increment = result_signum < 0;
                    break;
                case RoundingMode::HALF_UP:
                case RoundingMode::HALF_DOWN:
                case RoundingMode::HALF_EVEN: {
                    const BigInteger doubled_rem = rem.abs() << 1;
                    const BigInteger denominator_magnitude = denominator.abs();
                    increment = doubled_rem > denominator_magnitude
                        || (doubled_rem == denominator_magnitude
                            && (rounding_mode == RoundingMode::HALF_UP
                                || (rounding_mode == RoundingMode::HALF_EVEN && quotient.test_bit(0))));
                    break;
                }
                case RoundingMode::UNNECESSARY:
                    throw ArithmeticException("Rounding necessary.");
            }
            if (!increment) {
                return quotient;
            }
            return result_signum >= 0
                ? quotient + BigInteger::ONE
                : quotient - BigInteger::ONE;
        }

        /**
         * @brief Returns the saturated value for a conversion whose string form overflowed
         * the target floating-point type: signed infinity for huge magnitudes and
         * signed zero for tiny ones.
         * @tparam F The target floating-point type.
         * @return The signed infinity or signed zero saturating value.
         */
        template <FloatingPoint F>
        [[nodiscard]]
        F overflowed_value() const noexcept {
            const i64 adjusted = static_cast<i64>(precision()) - 1 - scale_val;
            if (adjusted > 0) {
                return signum() < 0
                    ? -NumericLimits<F>::infinity()
                    : NumericLimits<F>::infinity();
            }
            return signum() < 0 ? -F(0) : F(0);
        }

        /**
         * @brief Shared implementation of to_string() and to_engineering_string().
         * @param engineering Whether to use engineering notation.
         * @return The formatted string representation.
         */
        [[nodiscard]]
        String layout(bool engineering) const {
            const String coefficient = int_val.abs().to_string();
            const bool negative = int_val.signum() < 0;
            if (scale_val == 0) {
                return negative ? "-" + coefficient : coefficient;
            }
            const i64 adjusted = -static_cast<i64>(scale_val)
                + (static_cast<i64>(coefficient.size()) - 1);
            String result;
            if (negative) {
                result += '-';
            }
            if (scale_val > 0 && adjusted >= -6) {
                const i64 point = static_cast<i64>(coefficient.size()) - scale_val;
                if (point <= 0) {
                    result += "0.";
                    result.append(static_cast<usize>(-point), '0');
                    result += coefficient;
                } else {
                    result += coefficient.substr(0, static_cast<usize>(point));
                    result += '.';
                    result += coefficient.substr(static_cast<usize>(point));
                }
            } else if (!engineering) {
                result += coefficient[0];
                if (coefficient.size() > 1) {
                    result += '.';
                    result += coefficient.substr(1);
                }
                append_exponent(result, adjusted);
            } else if (int_val.signum() == 0) {
                const i64 value_exponent = -static_cast<i64>(scale_val);
                const i64 mod = ((value_exponent % 3) + 3) % 3;
                const i64 display_exponent = mod == 0 ? value_exponent : value_exponent + (3 - mod);
                result += '0';
                if (display_exponent != value_exponent) {
                    result += '.';
                    result.append(static_cast<usize>(display_exponent - value_exponent), '0');
                }
                append_exponent(result, display_exponent);
            } else {
                const i64 extra_digits = ((adjusted % 3) + 3) % 3;
                const i64 integer_digits = extra_digits + 1;
                if (integer_digits >= static_cast<i64>(coefficient.size())) {
                    result += coefficient;
                    result.append(static_cast<usize>(integer_digits) - coefficient.size(), '0');
                } else {
                    result += coefficient.substr(0, static_cast<usize>(integer_digits));
                    result += '.';
                    result += coefficient.substr(static_cast<usize>(integer_digits));
                }
                append_exponent(result, adjusted - extra_digits);
            }
            return result;
        }

        static void append_exponent(String& result, i64 exponent) {
            if (exponent == 0) {
                return;
            }
            result += 'E';
            if (exponent > 0) {
                result += '+';
            }
            result += Long::to_string(exponent);
        }
    public:
        /**
         * @brief Constructs a BigDecimal with the value zero.
         */
        constexpr BigDecimal() noexcept = default;

        /**
         * @brief Constructs a BigDecimal from a built-in integral value, with a scale of zero.
         * @tparam T The integral type of the value.
         * @param value The value to construct from.
         */
        template <Integral T>
        BigDecimal(T value):
            int_val{value} {}

        /**
         * @brief Constructs a BigDecimal from a BigInteger, with a scale of zero.
         * @param value The unscaled value.
         */
        BigDecimal(BigInteger value) noexcept:
            int_val{Ops::move(value)} {}

        /**
         * @brief Constructs a BigDecimal from an unscaled BigInteger value and a scale.
         * @param unscaled The unscaled value.
         * @param scale_value The scale; the value is unscaled * 10^-scale_value.
         */
        BigDecimal(BigInteger unscaled, i32 scale_value) noexcept:
            int_val{Ops::move(unscaled)}, scale_val{scale_value} {}

        /**
         * @brief Constructs a BigDecimal from its string representation.
         * @param value The string representation.
         * @throws NumberFormatException If the string is not a valid BigDecimal.
         *
         * Accepts an optional sign, a significand with an optional decimal point,
         * and an optional exponent.
         */
        explicit BigDecimal(StringView value) throws (NumberFormatException) {
            usize position = 0;
            bool negative = false;
            if (position < value.size() && (value[position] == '+' || value[position] == '-')) {
                negative = value[position] == '-';
                ++position;
            }
            String digits;
            digits.reserve(value.size());
            while (position < value.size() && value[position] >= '0' && value[position] <= '9') {
                digits += value[position];
                ++position;
            }
            i64 fraction_digits = 0;
            if (position < value.size() && value[position] == '.') {
                ++position;
                while (position < value.size() && value[position] >= '0' && value[position] <= '9') {
                    digits += value[position];
                    ++position;
                    ++fraction_digits;
                }
            }
            if (digits.empty()) {
                throw NumberFormatException("No digits in BigDecimal.");
            }
            i64 exponent = 0;
            if (position < value.size() && (value[position] == 'e' || value[position] == 'E')) {
                ++position;
                bool exponent_negative = false;
                if (position < value.size() && (value[position] == '+' || value[position] == '-')) {
                    exponent_negative = value[position] == '-';
                    ++position;
                }
                if (position == value.size() || value[position] < '0' || value[position] > '9') {
                    throw NumberFormatException("No exponent digits.");
                }
                const auto [ptr, ec] = stdx::text::from_chars(
                    value.data() + position,
                    value.data() + value.size(),
                    exponent
                );
                if (ec != Errc::SUCCESS || ptr != value.data() + value.size()) {
                    throw NumberFormatException("Invalid exponent.");
                }
                if (exponent_negative) {
                    exponent = -exponent;
                }
            } else if (position != value.size()) {
                throw NumberFormatException("Invalid character in BigDecimal.");
            }
            const i64 scale_value = fraction_digits - exponent;
            if (scale_value > Integer::MAX_VALUE || scale_value < Integer::MIN_VALUE) {
                throw NumberFormatException("Scale out of range.");
            }
            BigInteger unscaled(digits, 10);
            if (negative) {
                unscaled = -unscaled;
            }
            int_val = Ops::move(unscaled);
            scale_val = static_cast<i32>(scale_value);
        }

        /**
         * @brief Constructs a BigDecimal that is the exact decimal expansion of the
         * binary value of the given f64.
         * @param value The f64 value to expand exactly.
         * @throws NumberFormatException If value is infinite or NaN.
         *
         * Use value_of(f64) for the shortest decimal representation instead.
         */
        explicit BigDecimal(f64 value) throws (NumberFormatException) {
            if (!Math::is_finite(value)) {
                throw NumberFormatException("Infinite or NaN.");
            }
            const u64 bits = bit_cast<u64>(value);
            const bool negative = (bits >> 63) != 0;
            const i32 raw_exponent = static_cast<i32>((bits >> 52) & 0x7FF);
            u64 mantissa = bits & 0xFFFFFFFFFFFFFull;
            i32 exponent = -1074;
            if (raw_exponent != 0) {
                mantissa |= 1ull << 52;
                exponent = raw_exponent - 1075;
            }
            if (mantissa == 0) {
                return;
            }
            while ((mantissa & 1) == 0) {
                mantissa >>= 1;
                ++exponent;
            }
            BigInteger unscaled(mantissa);
            if (exponent >= 0) {
                unscaled = unscaled << exponent;
            } else {
                unscaled = unscaled * BigInteger(5).pow(-exponent);
                scale_val = -exponent;
            }
            int_val = negative ? -unscaled : Ops::move(unscaled);
        }

        static const BigDecimal ZERO; ///< The BigDecimal constant zero (scale 0).
        static const BigDecimal ONE; ///< The BigDecimal constant one (scale 0).
        static const BigDecimal TEN; ///< The BigDecimal constant ten (scale 0).

        /**
         * @brief Returns a BigDecimal whose value is equal to that of the given i64.
         * @param value The value to convert.
         * @return A BigDecimal with the same value as the given i64.
         */
        [[nodiscard]]
        static BigDecimal value_of(i64 value) {
            return BigDecimal(value);
        }

        /**
         * @brief Returns a BigDecimal with the given unscaled value and scale.
         * @param unscaled The unscaled value.
         * @param scale_value The scale.
         * @return A BigDecimal with the given unscaled value and scale.
         */
        [[nodiscard]]
        static BigDecimal value_of(i64 unscaled, i32 scale_value) {
            return BigDecimal(BigInteger(unscaled), scale_value);
        }

        /**
         * @brief Returns a BigDecimal equal to the shortest decimal representation
         * of the given f64.
         * @param value The f64 value to convert.
         * @return A BigDecimal equal to the shortest decimal representation of value.
         * @throws NumberFormatException If value is infinite or NaN.
         */
        [[nodiscard]]
        static BigDecimal value_of(f64 value) throws (NumberFormatException) {
            if (!Math::is_finite(value)) {
                throw NumberFormatException("Infinite or NaN.");
            }
            return BigDecimal(Double::to_string(value));
        }

        /**
         * @brief Parses a BigDecimal from a string.
         * @param value The string representation.
         * @return A BigDecimal with the parsed value.
         * @throws NumberFormatException If the string is not a valid BigDecimal.
         */
        [[nodiscard]]
        static BigDecimal parse(StringView value) throws (NumberFormatException) {
            return BigDecimal(value);
        }

        /**
         * @brief Parses a BigDecimal from a string, returning nullopt on failure.
         * @param value The string representation.
         * @return An Optional containing the parsed BigDecimal, or nullopt on failure.
         */
        [[nodiscard]]
        static Optional<BigDecimal> try_parse(StringView value) noexcept {
            try {
                return BigDecimal(value);
            } catch (const NumberFormatException&) {
                return nullopt;
            }
        }

        /**
         * @brief Returns (this / divisor) with the given result scale and rounding mode.
         * @param divisor The divisor.
         * @param result_scale The scale of the result.
         * @param rounding_mode The rounding mode to apply.
         * @return The quotient with the given scale.
         * @throws ArithmeticException If divisor is zero or rounding is necessary under UNNECESSARY.
         */
        [[nodiscard]]
        BigDecimal divide(
            const BigDecimal& divisor,
            i32 result_scale,
            RoundingMode rounding_mode
        ) const throws (ArithmeticException) {
            if (divisor.signum() == 0) {
                throw ArithmeticException("Division by zero.");
            }
            const i64 shift = static_cast<i64>(result_scale) - scale_val + divisor.scale_val;
            BigInteger numerator = int_val;
            BigInteger denominator = divisor.int_val;
            if (shift >= 0) {
                numerator = numerator * pow_ten(shift);
            } else {
                denominator = denominator * pow_ten(-shift);
            }
            return BigDecimal(divide_rounded(numerator, denominator, rounding_mode), result_scale);
        }

        /**
         * @brief Returns (this / divisor) with this BigDecimal's scale and the given
         * rounding mode.
         * @param divisor The divisor.
         * @param rounding_mode The rounding mode to apply.
         * @return The quotient with this BigDecimal's scale.
         * @throws ArithmeticException If divisor is zero or rounding is necessary under UNNECESSARY.
         */
        [[nodiscard]]
        BigDecimal divide(
            const BigDecimal& divisor,
            RoundingMode rounding_mode
        ) const throws (ArithmeticException) {
            return divide(divisor, scale_val, rounding_mode);
        }

        /**
         * @brief Returns the integer part of (this / divisor), with preferred scale
         * (this.scale() - divisor.scale()).
         * @param divisor The divisor.
         * @return The integer part of the quotient.
         * @throws ArithmeticException If divisor is zero.
         */
        [[nodiscard]]
        BigDecimal divide_to_integral_value(const BigDecimal& divisor) const throws (ArithmeticException) {
            if (divisor.signum() == 0) {
                throw ArithmeticException("Division by zero.");
            }
            const i64 preferred = static_cast<i64>(scale_val) - divisor.scale_val;
            const i64 diff = static_cast<i64>(divisor.scale_val) - scale_val;
            BigInteger numerator = int_val;
            BigInteger denominator = divisor.int_val;
            if (diff > 0) {
                numerator = numerator * pow_ten(diff);
            } else if (diff < 0) {
                denominator = denominator * pow_ten(-diff);
            }
            BigInteger quotient = numerator / denominator;
            if (quotient.signum() == 0) {
                return BigDecimal(BigInteger::ZERO, saturate_scale(preferred));
            }
            i64 result_scale = 0;
            if (preferred > 0) {
                quotient = quotient * pow_ten(preferred);
                result_scale = preferred;
            } else {
                while (result_scale > preferred) {
                    auto [next, rem] = quotient.divide_and_remainder(BigInteger::TEN);
                    if (rem.signum() != 0) {
                        break;
                    }
                    quotient = Ops::move(next);
                    --result_scale;
                }
            }
            return BigDecimal(Ops::move(quotient), check_scale(result_scale));
        }

        /**
         * @brief Returns the pair (integral quotient, remainder) of (this / divisor).
         * @param divisor The divisor.
         * @return The pair (integral quotient, remainder).
         * @throws ArithmeticException If divisor is zero.
         */
        [[nodiscard]]
        Pair<BigDecimal, BigDecimal> divide_and_remainder(const BigDecimal& divisor) const throws (ArithmeticException) {
            BigDecimal quotient = divide_to_integral_value(divisor);
            BigDecimal rem = *this - quotient * divisor;
            return {Ops::move(quotient), Ops::move(rem)};
        }

        /**
         * @brief Returns a BigDecimal whose value is (this ^ exponent), with the
         * exponent in [0, 999999999].
         * @param exponent The exponent, in the range [0, 999999999].
         * @return The value of this raised to the given power.
         * @throws ArithmeticException If exponent is out of range or the scale overflows.
         */
        [[nodiscard]]
        BigDecimal pow(i32 exponent) const throws (ArithmeticException) {
            if (exponent < 0 || exponent > 999999999) {
                throw ArithmeticException("Invalid exponent.");
            }
            return BigDecimal(
                int_val.pow(exponent),
                check_scale(static_cast<i64>(scale_val) * exponent)
            );
        }

        /**
         * @brief Returns a BigDecimal whose value is the absolute value of this.
         * @return The absolute value of this BigDecimal.
         */
        [[nodiscard]]
        BigDecimal abs() const {
            return signum() < 0 ? -(*this) : *this;
        }

        /**
         * @brief Returns the signum of this BigDecimal: -1, 0 or 1.
         * @return The signum: -1, 0 or 1.
         */
        [[nodiscard]]
        i32 signum() const noexcept {
            return int_val.signum();
        }

        /**
         * @brief Returns the scale: the number of digits to the right of the decimal
         * point (negative for trailing zero multiplication).
         * @return The scale of this BigDecimal.
         */
        [[nodiscard]]
        i32 scale() const noexcept {
            return scale_val;
        }

        /**
         * @brief Returns the number of significant decimal digits.
         * @return The number of significant decimal digits.
         */
        [[nodiscard]]
        i32 precision() const {
            if (int_val.signum() == 0) {
                return 1;
            }
            const BigInteger magnitude = int_val.abs();
            const i32 bits = magnitude.bit_length();
            i64 digits = static_cast<i64>(static_cast<f64>(bits - 1) * 0.30102999566398120) + 1;
            if (digits > 1 && magnitude < pow_ten(digits - 1)) {
                --digits;
            } else if (magnitude >= pow_ten(digits)) {
                ++digits;
            }
            return static_cast<i32>(digits);
        }

        /**
         * @brief Returns the unscaled value, this * 10^scale().
         * @return The unscaled value.
         */
        [[nodiscard]]
        BigInteger unscaled_value() const {
            return int_val;
        }

        /**
         * @brief Rounds this BigDecimal to the precision of the given MathContext.
         * @param context The MathContext giving the precision and rounding mode.
         * @return This BigDecimal rounded to the context's precision.
         * @throws ArithmeticException If the context's rounding mode is UNNECESSARY but rounding is required.
         */
        [[nodiscard]]
        BigDecimal round(const MathContext& context) const throws (ArithmeticException) {
            if (context.precision() == 0) {
                return *this;
            }
            BigDecimal result = *this;
            while (result.int_val.signum() != 0
                && result.precision() > static_cast<i32>(context.precision())) {
                const i64 drop = static_cast<i64>(result.precision())
                    - static_cast<i32>(context.precision());
                result = BigDecimal(
                    divide_rounded(result.int_val, pow_ten(drop), context.rounding_mode()),
                    check_scale(static_cast<i64>(result.scale_val) - drop)
                );
            }
            return result;
        }

        /**
         * @brief Returns a BigDecimal with the given scale, rounding the value if needed.
         * @param new_scale The desired scale.
         * @param rounding_mode The rounding mode to apply.
         * @return This BigDecimal with the given scale.
         * @throws ArithmeticException If rounding is necessary under UNNECESSARY.
         */
        [[nodiscard]]
        BigDecimal set_scale(
            i32 new_scale,
            RoundingMode rounding_mode
        ) const throws (ArithmeticException) {
            if (new_scale == scale_val) {
                return *this;
            }
            if (int_val.signum() == 0) {
                return BigDecimal(BigInteger::ZERO, new_scale);
            }
            const i64 diff = static_cast<i64>(new_scale) - scale_val;
            if (diff > 0) {
                return BigDecimal(int_val * pow_ten(diff), new_scale);
            }
            return BigDecimal(divide_rounded(int_val, pow_ten(-diff), rounding_mode), new_scale);
        }

        /**
         * @brief Returns a BigDecimal with the given scale, throwing if rounding
         * would be required.
         * @param new_scale The desired scale.
         * @return This BigDecimal with the given scale.
         * @throws ArithmeticException If changing the scale would require rounding.
         */
        [[nodiscard]]
        BigDecimal set_scale(i32 new_scale) const throws (ArithmeticException) {
            return set_scale(new_scale, RoundingMode::UNNECESSARY);
        }

        /**
         * @brief Returns a BigDecimal equivalent to this with the decimal point moved
         * the given number of places to the left.
         * @param distance The number of places to move the decimal point left.
         * @return This BigDecimal with the decimal point moved left.
         * @throws ArithmeticException If the resulting scale overflows.
         */
        [[nodiscard]]
        BigDecimal move_point_left(i32 distance) const throws (ArithmeticException) {
            const i64 new_scale = static_cast<i64>(scale_val) + distance;
            return new_scale >= 0
                ? BigDecimal(int_val, check_scale(new_scale))
                : BigDecimal(int_val * pow_ten(-new_scale), 0);
        }

        /**
         * @brief Returns a BigDecimal equivalent to this with the decimal point moved
         * the given number of places to the right.
         * @param distance The number of places to move the decimal point right.
         * @return This BigDecimal with the decimal point moved right.
         * @throws ArithmeticException If the resulting scale overflows.
         */
        [[nodiscard]]
        BigDecimal move_point_right(i32 distance) const throws (ArithmeticException) {
            const i64 new_scale = static_cast<i64>(scale_val) - distance;
            return new_scale >= 0
                ? BigDecimal(int_val, check_scale(new_scale))
                : BigDecimal(int_val * pow_ten(-new_scale), 0);
        }

        /**
         * @brief Returns a BigDecimal whose value is (this * 10^distance).
         * @param distance The power of ten to scale by.
         * @return This BigDecimal scaled by 10^distance.
         * @throws ArithmeticException If the resulting scale overflows.
         */
        [[nodiscard]]
        BigDecimal scale_by_power_of_ten(i32 distance) const throws (ArithmeticException) {
            return BigDecimal(int_val, check_scale(static_cast<i64>(scale_val) - distance));
        }

        /**
         * @brief Returns a numerically equal BigDecimal with all trailing zeros
         * removed from the representation.
         * @return A numerically equal BigDecimal with trailing zeros stripped.
         */
        [[nodiscard]]
        BigDecimal strip_trailing_zeros() const {
            if (int_val.signum() == 0) {
                return ZERO;
            }
            BigInteger value = int_val;
            i64 result_scale = scale_val;
            while (result_scale > Integer::MIN_VALUE) {
                auto [quotient, rem] = value.divide_and_remainder(BigInteger::TEN);
                if (rem.signum() != 0) {
                    break;
                }
                value = Ops::move(quotient);
                --result_scale;
            }
            return BigDecimal(Ops::move(value), static_cast<i32>(result_scale));
        }

        /**
         * @brief Returns the size of an ulp (unit in the last place) of this BigDecimal.
         * @return The size of an ulp of this BigDecimal.
         */
        [[nodiscard]]
        BigDecimal ulp() const {
            return BigDecimal(BigInteger::ONE, scale_val);
        }

        /**
         * @brief Checks that numerical value and scale are exactly equal.
         * @param other The value to compare against.
         * @return True if both the value and scale are equal.
         */
        [[nodiscard]]
        bool equals(const BigDecimal& other) const noexcept {
            return scale_val == other.scale_val && int_val == other.int_val;
        }

        [[nodiscard]]
        bool operator==(const BigDecimal& other) const {
            return (*this <=> other) == 0;
        }

        /**
         * @brief Compares this BigDecimal with another numerically, ignoring scale
         * differences (so 2.0 and 2.00 are equivalent).
         * @param other The value to compare against.
         * @return The weak ordering of the numeric values.
         */
        [[nodiscard]]
        WeakOrdering operator<=>(const BigDecimal& other) const {
            const i32 this_signum = signum();
            const i32 other_signum = other.signum();
            if (this_signum != other_signum) {
                return this_signum < other_signum ? WeakOrdering::LESS : WeakOrdering::GREATER;
            }
            if (this_signum == 0) {
                return WeakOrdering::EQUIVALENT;
            }
            if (scale_val == other.scale_val) {
                return compare(int_val, other.int_val);
            }
            // Compare adjusted exponents first so wildly different magnitudes never
            // require materialising a huge power of ten for scale alignment.
            const i64 this_adjusted = static_cast<i64>(precision()) - scale_val;
            const i64 other_adjusted = static_cast<i64>(other.precision()) - other.scale_val;
            if (this_adjusted != other_adjusted) {
                const bool less = (this_adjusted < other_adjusted) == (this_signum > 0);
                return less ? WeakOrdering::LESS : WeakOrdering::GREATER;
            }
            const i64 diff = static_cast<i64>(scale_val) - other.scale_val;
            return diff > 0
                ? compare(int_val, other.int_val * pow_ten(diff))
                : compare(int_val * pow_ten(-diff), other.int_val);
        }

        /**
         * @brief Converts this BigDecimal to a BigInteger, discarding any fraction.
         * @return The integer part of this BigDecimal.
         */
        [[nodiscard]]
        BigInteger to_big_integer() const {
            if (scale_val == 0) {
                return int_val;
            }
            if (scale_val > 0) {
                return int_val / pow_ten(scale_val);
            }
            return int_val * pow_ten(-static_cast<i64>(scale_val));
        }

        /**
         * @brief Converts this BigDecimal to a BigInteger, throwing if it has a
         * nonzero fractional part.
         * @return The exact integer value of this BigDecimal.
         * @throws ArithmeticException If this BigDecimal has a nonzero fractional part.
         */
        [[nodiscard]]
        BigInteger to_big_integer_exact() const throws (ArithmeticException) {
            if (scale_val <= 0) {
                return to_big_integer();
            }
            auto [quotient, rem] = int_val.divide_and_remainder(pow_ten(scale_val));
            if (rem.signum() != 0) {
                throw ArithmeticException("Rounding necessary.");
            }
            return quotient;
        }

        /**
         * @brief Converts this BigDecimal to an i32: the fraction is discarded and
         * only the low-order 32 bits of the integer part are kept.
         * @return The value of this BigDecimal as an i32.
         */
        [[nodiscard]]
        i32 int_value() const {
            return to_big_integer().int_value();
        }

        /**
         * @brief Converts this BigDecimal to an i64: the fraction is discarded and
         * only the low-order 64 bits of the integer part are kept.
         * @return The value of this BigDecimal as an i64.
         */
        [[nodiscard]]
        i64 long_value() const {
            return to_big_integer().long_value();
        }

        /**
         * @brief Converts this BigDecimal to an i8, throwing if it has a fractional
         * part or does not fit.
         * @return The value of this BigDecimal as an i8.
         * @throws ArithmeticException If this has a fractional part or does not fit in an i8.
         */
        [[nodiscard]]
        i8 byte_value_exact() const throws (ArithmeticException) {
            return to_big_integer_exact().byte_value_exact();
        }

        /**
         * @brief Converts this BigDecimal to an i16, throwing if it has a fractional
         * part or does not fit.
         * @return The value of this BigDecimal as an i16.
         * @throws ArithmeticException If this has a fractional part or does not fit in an i16.
         */
        [[nodiscard]]
        i16 short_value_exact() const throws (ArithmeticException) {
            return to_big_integer_exact().short_value_exact();
        }

        /**
         * @brief Converts this BigDecimal to an i32, throwing if it has a fractional
         * part or does not fit.
         * @return The value of this BigDecimal as an i32.
         * @throws ArithmeticException If this has a fractional part or does not fit in an i32.
         */
        [[nodiscard]]
        i32 int_value_exact() const throws (ArithmeticException) {
            return to_big_integer_exact().int_value_exact();
        }

        /**
         * @brief Converts this BigDecimal to an i64, throwing if it has a fractional
         * part or does not fit.
         * @return The value of this BigDecimal as an i64.
         * @throws ArithmeticException If this has a fractional part or does not fit in an i64.
         */
        [[nodiscard]]
        i64 long_value_exact() const throws (ArithmeticException) {
            return to_big_integer_exact().long_value_exact();
        }

        /**
         * @brief Converts this BigDecimal to an f32, rounding to nearest.
         * @return The value of this BigDecimal as an f32.
         */
        [[nodiscard]]
        f32 float_value() const {
            const String repr = to_string();
            f32 result = 0.0f;
            const auto [ptr, ec] = stdx::text::from_chars(repr.data(), repr.data() + repr.size(), result);
            if (ec != Errc::SUCCESS) {
                return overflowed_value<f32>();
            }
            return result;
        }

        /**
         * @brief Converts this BigDecimal to an f64, rounding to nearest.
         * @return The value of this BigDecimal as an f64.
         */
        [[nodiscard]]
        f64 double_value() const {
            const String repr = to_string();
            f64 result = 0.0;
            const auto [ptr, ec] = stdx::text::from_chars(repr.data(), repr.data() + repr.size(), result);
            if (ec != Errc::SUCCESS) {
                return overflowed_value<f64>();
            }
            return result;
        }

        /**
         * @brief Returns the string representation, using scientific notation.
         * @return The string representation of this BigDecimal.
         */
        [[nodiscard]]
        String to_string() const {
            return layout(false);
        }

        /**
         * @brief Returns the string representation without an exponent field.
         * @return The plain string representation of this BigDecimal.
         */
        [[nodiscard]]
        String to_plain_string() const {
            if (scale_val == 0) {
                return int_val.to_string();
            }
            if (scale_val < 0) {
                if (int_val.signum() == 0) {
                    return "0";
                }
                String result = int_val.to_string();
                result.append(static_cast<usize>(-static_cast<i64>(scale_val)), '0');
                return result;
            }
            const String coefficient = int_val.abs().to_string();
            String result;
            if (int_val.signum() < 0) {
                result += '-';
            }
            const i64 point = static_cast<i64>(coefficient.size()) - scale_val;
            if (point <= 0) {
                result += "0.";
                result.append(static_cast<usize>(-point), '0');
                result += coefficient;
            } else {
                result += coefficient.substr(0, static_cast<usize>(point));
                result += '.';
                result += coefficient.substr(static_cast<usize>(point));
            }
            return result;
        }

        /**
         * @brief Returns the string representation, using an exponent that is a
         * multiple of three when exponential notation is needed.
         * @return The engineering-notation string representation of this BigDecimal.
         */
        [[nodiscard]]
        String to_engineering_string() const {
            return layout(true);
        }

        /**
         * @brief Returns a BigDecimal whose value is (lhs + rhs) and whose scale is
         * the maximum of the two scales.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The sum (lhs + rhs).
         */
        [[nodiscard]]
        friend BigDecimal operator+(const BigDecimal& lhs, const BigDecimal& rhs) {
            const i32 result_scale = Math::max(lhs.scale_val, rhs.scale_val);
            return BigDecimal(
                lhs.aligned_unscaled(result_scale) + rhs.aligned_unscaled(result_scale),
                result_scale
            );
        }

        /**
         * @brief Returns a BigDecimal whose value is (lhs - rhs) and whose scale is
         * the maximum of the two scales.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The difference (lhs - rhs).
         */
        [[nodiscard]]
        friend BigDecimal operator-(const BigDecimal& lhs, const BigDecimal& rhs) {
            const i32 result_scale = Math::max(lhs.scale_val, rhs.scale_val);
            return BigDecimal(
                lhs.aligned_unscaled(result_scale) - rhs.aligned_unscaled(result_scale),
                result_scale
            );
        }

        /**
         * @brief Returns a BigDecimal whose value is (lhs * rhs) and whose scale is
         * the sum of the two scales.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The product (lhs * rhs).
         * @throws ArithmeticException If the resulting scale overflows.
         */
        [[nodiscard]]
        friend BigDecimal operator*(const BigDecimal& lhs, const BigDecimal& rhs) throws (ArithmeticException) {
            return BigDecimal(
                lhs.int_val * rhs.int_val,
                check_scale(static_cast<i64>(lhs.scale_val) + rhs.scale_val)
            );
        }

        /**
         * @brief Returns the exact quotient (lhs / rhs).
         * @param lhs The dividend.
         * @param rhs The divisor.
         * @return The exact quotient (lhs / rhs).
         * @throws ArithmeticException If rhs is zero or the quotient has no terminating decimal expansion.
         *
         * The preferred scale of the result is (lhs.scale() - rhs.scale()); if the
         * exact quotient requires more digits the scale is extended, and if it has
         * no terminating decimal expansion an ArithmeticException is thrown. Use
         * divide(divisor, scale, rounding_mode) to round instead.
         */
        [[nodiscard]]
        friend BigDecimal operator/(const BigDecimal& lhs, const BigDecimal& rhs) throws (ArithmeticException) {
            if (rhs.signum() == 0) {
                throw ArithmeticException("Division by zero.");
            }
            const i64 preferred = static_cast<i64>(lhs.scale_val) - rhs.scale_val;
            if (lhs.signum() == 0) {
                return BigDecimal(BigInteger::ZERO, saturate_scale(preferred));
            }
            BigInteger numerator = lhs.int_val;
            BigInteger denominator = rhs.int_val;
            const BigInteger common = numerator.gcd(denominator);
            numerator = numerator / common;
            denominator = denominator / common;
            if (denominator.signum() < 0) {
                numerator = -numerator;
                denominator = -denominator;
            }
            const i32 twos = denominator.get_lowest_set_bit();
            denominator = denominator >> twos;
            i64 fives = 0;
            const BigInteger five(5);
            while (true) {
                auto [quotient, rem] = denominator.divide_and_remainder(five);
                if (rem.signum() != 0) {
                    break;
                }
                denominator = Ops::move(quotient);
                ++fives;
            }
            if (denominator != BigInteger::ONE) {
                throw ArithmeticException("Non-terminating decimal expansion.");
            }
            const i64 extra = Math::max(static_cast<i64>(twos), fives);
            numerator = numerator
                * BigInteger::TWO.pow(static_cast<i32>(extra - twos))
                * five.pow(static_cast<i32>(extra - fives));
            return BigDecimal(Ops::move(numerator), check_scale(preferred + extra));
        }

        /**
         * @brief Returns (lhs % rhs), defined as
         * lhs - lhs.divide_to_integral_value(rhs) * rhs.
         * @param lhs The dividend.
         * @param rhs The divisor.
         * @return The remainder (lhs % rhs).
         * @throws ArithmeticException If rhs is zero.
         */
        [[nodiscard]]
        friend BigDecimal operator%(const BigDecimal& lhs, const BigDecimal& rhs) throws (ArithmeticException) {
            return lhs - lhs.divide_to_integral_value(rhs) * rhs;
        }

        /**
         * @brief Returns a BigDecimal whose value is (-this).
         * @return The negation of this BigDecimal.
         */
        [[nodiscard]]
        BigDecimal operator-() const {
            return BigDecimal(-int_val, scale_val);
        }

        [[nodiscard]]
        BigDecimal operator+() const {
            return *this;
        }

        BigDecimal& operator+=(const BigDecimal& other) {
            *this = *this + other;
            return *this;
        }

        BigDecimal& operator-=(const BigDecimal& other) {
            *this = *this - other;
            return *this;
        }

        BigDecimal& operator*=(const BigDecimal& other) throws (ArithmeticException) {
            *this = *this * other;
            return *this;
        }

        BigDecimal& operator/=(const BigDecimal& other) throws (ArithmeticException) {
            *this = *this / other;
            return *this;
        }

        BigDecimal& operator%=(const BigDecimal& other) throws (ArithmeticException) {
            *this = *this % other;
            return *this;
        }
    };

    inline const BigDecimal BigDecimal::ZERO{0};
    inline const BigDecimal BigDecimal::ONE{1};
    inline const BigDecimal BigDecimal::TEN{10};

    /**
     * @brief Returns a BigDecimal whose value is the absolute value of the argument.
     * @param value The BigDecimal to take the absolute value of.
     * @return The absolute value of the argument.
     */
    [[nodiscard]]
    inline BigDecimal abs(const BigDecimal& value) {
        return value.abs();
    }

    /**
     * @brief Returns the smaller of two BigDecimal values.
     * @param a The first value.
     * @param b The second value.
     * @return The smaller of a and b.
     */
    [[nodiscard]]
    inline const BigDecimal& min(const BigDecimal& a, const BigDecimal& b) {
        return b < a ? b : a;
    }

    /**
     * @brief Returns the larger of two BigDecimal values.
     * @param a The first value.
     * @param b The second value.
     * @return The larger of a and b.
     */
    [[nodiscard]]
    inline const BigDecimal& max(const BigDecimal& a, const BigDecimal& b) {
        return a < b ? b : a;
    }

    /**
     * @brief Clamps value to the range [low, high].
     * @param value The value to clamp.
     * @param low The lower bound.
     * @param high The upper bound.
     * @return value clamped to [low, high].
     */
    [[nodiscard]]
    inline const BigDecimal& clamp(const BigDecimal& value, const BigDecimal& low, const BigDecimal& high) {
        return value < low ? low : (high < value ? high : value);
    }

    /**
     * @brief Returns the signum of the BigDecimal: -1, 0 or 1.
     * @param value The BigDecimal to inspect.
     * @return The signum: -1, 0 or 1.
     */
    [[nodiscard]]
    inline i32 signum(const BigDecimal& value) noexcept {
        return value.signum();
    }

    /**
     * @brief Returns base raised to the given exponent, in the range [0, 999999999].
     * @param base The base.
     * @param exponent The exponent, in the range [0, 999999999].
     * @return The value of base raised to the given power.
     * @throws ArithmeticException If exponent is out of range or the scale overflows.
     */
    [[nodiscard]]
    inline BigDecimal pow(const BigDecimal& base, i32 exponent) throws (ArithmeticException) {
        return base.pow(exponent);
    }
}

using stdx::math::BigDecimal;

namespace stdx {
    namespace core {
        template <>
        struct Hash<BigDecimal> {
            [[nodiscard]]
            usize operator()(const BigDecimal& value) const noexcept {
                return value.strip_trailing_zeros().hash_code();
            }
        };
    }

    namespace fmt {
        template <>
        struct Formatter<BigDecimal> {
            static constexpr const char* parse(FormatParseContext& ctx) noexcept {
                return ctx.begin();
            }

            static FormatContext::iterator format(const BigDecimal& value, FormatContext& ctx) {
                return format_to(ctx.out(), "{}", value.to_string());
            }
        };
    }
}

template <>
struct stdx::core::hash<BigDecimal> : public stdx::core::Hash<BigDecimal> {};

template <>
struct stdx::fmt::formatter<BigDecimal> : public stdx::fmt::Formatter<BigDecimal> {};

