#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::math
 * @brief Standard library mathematical operations.
 */
export namespace stdx::math {
    /**
     * @class BigInteger
     * @brief Immutable arbitrary-precision integer.
     *
     * The value is stored in sign-magnitude form: a signum of -1, 0 or 1 and a
     * normalized little-endian vector of 32-bit limbs (no trailing zero limbs,
     * an empty vector if and only if the value is zero).
     */
    class [[nodiscard]] BigInteger final {
    private:
        Vector<u32> mag; ///< The magnitude, little-endian with no trailing zero limbs.
        i32 sign = 0; ///< The signum: -1, 0 or 1.

        static constexpr usize KARATSUBA_THRESHOLD = 64uz;

        /**
         * @brief Constructs from a signum and an already normalized magnitude.
         * @param signum_value The signum: -1, 0 or 1.
         * @param magnitude The magnitude, little-endian with no trailing zero limbs.
         */
        BigInteger(i32 signum_value, Vector<u32>&& magnitude) noexcept:
            mag{Ops::move(magnitude)},
            sign{mag.empty() ? 0 : signum_value} {}

        /**
         * @brief Returns a hash code.
         * @return The hash code of this BigInteger.
         */
        [[nodiscard]]
        usize hash_code() const noexcept {
            u32 hash = 0;
            for (usize i = mag.size(); i-- > 0;) {
                hash = 31u * hash + mag[i];
            }
            return static_cast<usize>(hash) * sign;
        }

        friend struct stdx::core::Hash<BigInteger>;

        static void trim(Vector<u32>& magnitude) noexcept {
            while (!magnitude.empty() && magnitude.back() == 0) {
                magnitude.pop_back();
            }
        }

        [[nodiscard]]
        static usize mag_bit_length(const Vector<u32>& magnitude) noexcept {
            return magnitude.empty()
                ? 0
                : (magnitude.size() - 1) * 32 + static_cast<usize>(Math::bit_width(magnitude.back()));
        }

        [[nodiscard]]
        static bool mag_is_power_of_two(const Vector<u32>& magnitude) noexcept {
            if (magnitude.empty() || Math::count_pop(magnitude.back()) != 1) {
                return false;
            }
            for (usize i = 0; i + 1 < magnitude.size(); ++i) {
                if (magnitude[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]]
        static usize first_nonzero_index(const Vector<u32>& magnitude) noexcept {
            usize index = 0;
            while (index < magnitude.size() && magnitude[index] == 0) {
                ++index;
            }
            return index;
        }

        [[nodiscard]]
        static i32 mag_cmp(const Vector<u32>& lhs, const Vector<u32>& rhs) noexcept {
            if (lhs.size() != rhs.size()) {
                return lhs.size() < rhs.size() ? -1 : 1;
            }
            for (usize i = lhs.size(); i-- > 0;) {
                if (lhs[i] != rhs[i]) {
                    return lhs[i] < rhs[i] ? -1 : 1;
                }
            }
            return 0;
        }

        [[nodiscard]]
        static Vector<u32> mag_add(const Vector<u32>& lhs, const Vector<u32>& rhs) {
            const Vector<u32>& longer = lhs.size() >= rhs.size() ? lhs : rhs;
            const Vector<u32>& shorter = lhs.size() >= rhs.size() ? rhs : lhs;
            Vector<u32> result;
            result.reserve(longer.size() + 1);
            u64 carry = 0;
            for (usize i = 0; i < longer.size(); ++i) {
                u64 sum = static_cast<u64>(longer[i]) + carry;
                if (i < shorter.size()) {
                    sum += shorter[i];
                }
                result.push_back(static_cast<u32>(sum));
                carry = sum >> 32;
            }
            if (carry != 0) {
                result.push_back(1);
            }
            return result;
        }

        /**
         * @brief Subtracts rhs from lhs; requires lhs >= rhs.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The result of the subtraction.
         */
        [[nodiscard]]
        static Vector<u32> mag_sub(const Vector<u32>& lhs, const Vector<u32>& rhs) {
            Vector<u32> result;
            result.reserve(lhs.size());
            i64 borrow = 0;
            for (usize i = 0; i < lhs.size(); ++i) {
                i64 diff = static_cast<i64>(lhs[i]) - borrow
                    - (i < rhs.size() ? static_cast<i64>(rhs[i]) : 0);
                if (diff < 0) {
                    diff += 1LL << 32;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                result.push_back(static_cast<u32>(diff));
            }
            trim(result);
            return result;
        }

        /**
         * @brief Increments the magnitude in-place.
         * @param magnitude The magnitude to increment.
         */
        static void mag_increment(Vector<u32>& magnitude) {
            for (u32& limb: magnitude) {
                if (++limb != 0) {
                    return;
                }
            }
            magnitude.push_back(1);
        }

        /**
         * @brief Multiplies the magnitude by a single limb and adds an addend, in-place.
         * @param magnitude The magnitude to modify.
         * @param multiplier The single-limb multiplier.
         * @param addend The single-limb addend.
         */
        static void mag_mul_add(Vector<u32>& magnitude, u32 multiplier, u32 addend) {
            u64 carry = addend;
            for (u32& limb: magnitude) {
                const u64 product = static_cast<u64>(limb) * multiplier + carry;
                limb = static_cast<u32>(product);
                carry = product >> 32;
            }
            if (carry != 0) {
                magnitude.push_back(static_cast<u32>(carry));
            }
        }

        /**
         * @brief Divides the magnitude by a single limb; returns the remainder.
         * @param magnitude The magnitude to divide.
         * @param divisor The single-limb divisor.
         * @return The remainder of the division.
         */
        [[nodiscard]]
        static u32 mag_divrem_word(Vector<u32>& magnitude, u32 divisor) noexcept {
            u64 rem = 0;
            for (usize i = magnitude.size(); i-- > 0;) {
                const u64 current = (rem << 32) | magnitude[i];
                magnitude[i] = static_cast<u32>(current / divisor);
                rem = current % divisor;
            }
            trim(magnitude);
            return static_cast<u32>(rem);
        }

        [[nodiscard]]
        static Vector<u32> mag_slice(const Vector<u32>& magnitude, usize start, usize count) {
            if (start >= magnitude.size()) {
                return {};
            }
            const usize end = Math::min(magnitude.size(), start + count);
            Vector<u32> result(magnitude.begin() + static_cast<isize>(start), magnitude.begin() + static_cast<isize>(end));
            trim(result);
            return result;
        }

        /**
         * @brief Adds src (shifted left by offset limbs) into acc.
         * @param acc The accumulator to modify.
         * @param src The source vector to add.
         * @param offset The offset by which to shift the source vector.
         */
        static void mag_add_into(Vector<u32>& acc, const Vector<u32>& src, usize offset) {
            if (src.empty()) {
                return;
            }
            if (acc.size() < src.size() + offset) {
                acc.resize(src.size() + offset, 0);
            }
            u64 carry = 0;
            for (usize i = 0; i < src.size(); ++i) {
                const u64 sum = static_cast<u64>(acc[i + offset]) + src[i] + carry;
                acc[i + offset] = static_cast<u32>(sum);
                carry = sum >> 32;
            }
            usize index = offset + src.size();
            while (carry != 0) {
                if (index == acc.size()) {
                    acc.push_back(static_cast<u32>(carry));
                    break;
                }
                const u64 sum = static_cast<u64>(acc[index]) + carry;
                acc[index] = static_cast<u32>(sum);
                carry = sum >> 32;
                ++index;
            }
        }

        [[nodiscard]]
        static Vector<u32> mag_mul_schoolbook(const Vector<u32>& lhs, const Vector<u32>& rhs) {
            Vector<u32> result(lhs.size() + rhs.size(), 0);
            for (usize i = 0; i < lhs.size(); ++i) {
                u64 carry = 0;
                for (usize j = 0; j < rhs.size(); ++j) {
                    const u64 current = result[i + j] + static_cast<u64>(lhs[i]) * rhs[j] + carry;
                    result[i + j] = static_cast<u32>(current);
                    carry = current >> 32;
                }
                result[i + rhs.size()] = static_cast<u32>(carry);
            }
            trim(result);
            return result;
        }

        [[nodiscard]]
        static Vector<u32> mag_mul(const Vector<u32>& lhs, const Vector<u32>& rhs) {
            if (lhs.empty() || rhs.empty()) {
                return {};
            }
            if (Math::min(lhs.size(), rhs.size()) < KARATSUBA_THRESHOLD) {
                return mag_mul_schoolbook(lhs, rhs);
            }
            const usize half = (Math::max(lhs.size(), rhs.size()) + 1) / 2;
            const Vector<u32> lhs_low = mag_slice(lhs, 0, half);
            const Vector<u32> lhs_high = mag_slice(lhs, half, lhs.size());
            const Vector<u32> rhs_low = mag_slice(rhs, 0, half);
            const Vector<u32> rhs_high = mag_slice(rhs, half, rhs.size());
            const Vector<u32> low_product = mag_mul(lhs_low, rhs_low);
            const Vector<u32> high_product = mag_mul(lhs_high, rhs_high);
            Vector<u32> cross = mag_mul(mag_add(lhs_low, lhs_high), mag_add(rhs_low, rhs_high));
            cross = mag_sub(cross, mag_add(low_product, high_product));
            Vector<u32> result = low_product;
            mag_add_into(result, cross, half);
            mag_add_into(result, high_product, 2 * half);
            trim(result);
            return result;
        }

        /**
         * @brief Divides two magnitudes using Knuth's Algorithm D.
         * @param dividend The dividend magnitude.
         * @param divisor The divisor magnitude.
         * @param quotient The resulting quotient magnitude.
         * @param rem The resulting remainder magnitude.
         */
        static void mag_divrem(
            const Vector<u32>& dividend,
            const Vector<u32>& divisor,
            Vector<u32>& quotient,
            Vector<u32>& rem
        ) {
            static constexpr u64 BASE = 1ull << 32;
            quotient.clear();
            rem.clear();
            const i32 comparison = mag_cmp(dividend, divisor);
            if (comparison < 0) {
                rem = dividend;
                return;
            }
            if (comparison == 0) {
                quotient.assign(1, 1);
                return;
            }
            if (divisor.size() == 1) {
                quotient = dividend;
                const u32 word_rem = mag_divrem_word(quotient, divisor[0]);
                if (word_rem != 0) {
                    rem.assign(1, word_rem);
                }
                return;
            }
            const usize n = divisor.size();
            const usize m = dividend.size() - n;
            const u32 shift = static_cast<u32>(Math::count_left_zero(divisor.back()));
            Vector<u32> vn(n);
            Vector<u32> un(dividend.size() + 1, 0);
            if (shift > 0) {
                for (usize i = n - 1; i > 0; --i) {
                    vn[i] = (divisor[i] << shift) | (divisor[i - 1] >> (32 - shift));
                }
                vn[0] = divisor[0] << shift;
                un[dividend.size()] = dividend.back() >> (32 - shift);
                for (usize i = dividend.size() - 1; i > 0; --i) {
                    un[i] = (dividend[i] << shift) | (dividend[i - 1] >> (32 - shift));
                }
                un[0] = dividend[0] << shift;
            } else {
                for (usize i = 0; i < dividend.size(); ++i) {
                    un[i] = dividend[i];
                }
                vn = divisor;
            }
            quotient.assign(m + 1, 0);
            for (usize j = m + 1; j-- > 0;) {
                const u64 numerator = (static_cast<u64>(un[j + n]) << 32) | un[j + n - 1];
                u64 qhat = numerator / vn[n - 1];
                u64 rhat = numerator % vn[n - 1];
                while (qhat >= BASE || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
                    --qhat;
                    rhat += vn[n - 1];
                    if (rhat >= BASE) {
                        break;
                    }
                }
                i64 borrow = 0;
                u64 carry = 0;
                for (usize i = 0; i < n; ++i) {
                    const u64 product = qhat * vn[i] + carry;
                    carry = product >> 32;
                    const i64 diff = static_cast<i64>(un[i + j]) - static_cast<i64>(static_cast<u32>(product)) - borrow;
                    un[i + j] = static_cast<u32>(diff);
                    borrow = diff < 0 ? 1 : 0;
                }
                const i64 top = static_cast<i64>(un[j + n]) - static_cast<i64>(carry) - borrow;
                un[j + n] = static_cast<u32>(top);
                quotient[j] = static_cast<u32>(qhat);
                if (top < 0) {
                    --quotient[j];
                    u64 fix = 0;
                    for (usize i = 0; i < n; ++i) {
                        const u64 sum = static_cast<u64>(un[i + j]) + vn[i] + fix;
                        un[i + j] = static_cast<u32>(sum);
                        fix = sum >> 32;
                    }
                    un[j + n] += static_cast<u32>(fix);
                }
            }
            trim(quotient);
            rem.assign(n, 0);
            if (shift > 0) {
                for (usize i = 0; i + 1 < n; ++i) {
                    rem[i] = (un[i] >> shift) | (un[i + 1] << (32 - shift));
                }
                rem[n - 1] = un[n - 1] >> shift;
            } else {
                for (usize i = 0; i < n; ++i) {
                    rem[i] = un[i];
                }
            }
            trim(rem);
        }

        [[nodiscard]]
        static Vector<u32> mag_shl(const Vector<u32>& magnitude, usize distance) {
            if (magnitude.empty() || distance == 0) {
                return magnitude;
            }
            const usize limb_shift = distance / 32;
            const u32 bit_shift = static_cast<u32>(distance % 32);
            Vector<u32> result(magnitude.size() + limb_shift + (bit_shift != 0 ? 1 : 0), 0);
            if (bit_shift == 0) {
                for (usize i = 0; i < magnitude.size(); ++i) {
                    result[i + limb_shift] = magnitude[i];
                }
            } else {
                u32 carry = 0;
                for (usize i = 0; i < magnitude.size(); ++i) {
                    result[i + limb_shift] = (magnitude[i] << bit_shift) | carry;
                    carry = magnitude[i] >> (32 - bit_shift);
                }
                result[magnitude.size() + limb_shift] = carry;
            }
            trim(result);
            return result;
        }

        [[nodiscard]]
        static Vector<u32> mag_shr(const Vector<u32>& magnitude, usize distance, bool& lost_bits) {
            lost_bits = false;
            if (magnitude.empty()) {
                return {};
            }
            const usize limb_shift = distance / 32;
            const u32 bit_shift = static_cast<u32>(distance % 32);
            if (limb_shift >= magnitude.size()) {
                lost_bits = true;
                return {};
            }
            for (usize i = 0; i < limb_shift && !lost_bits; ++i) {
                lost_bits = magnitude[i] != 0;
            }
            if (!lost_bits && bit_shift != 0) {
                lost_bits = (magnitude[limb_shift] & ((1u << bit_shift) - 1)) != 0;
            }
            const usize result_size = magnitude.size() - limb_shift;
            Vector<u32> result(result_size);
            if (bit_shift == 0) {
                for (usize i = 0; i < result_size; ++i) {
                    result[i] = magnitude[i + limb_shift];
                }
            } else {
                for (usize i = 0; i < result_size; ++i) {
                    const u32 high = i + limb_shift + 1 < magnitude.size()
                        ? magnitude[i + limb_shift + 1]
                        : 0;
                    result[i] = (magnitude[i + limb_shift] >> bit_shift) | (high << (32 - bit_shift));
                }
            }
            trim(result);
            return result;
        }

        /**
         * @brief Shifts the magnitude of the BigInteger right by the specified distance.
         * @param distance The number of positions to shift.
         * @return The shifted BigInteger.
         */
        [[nodiscard]]
        BigInteger shift_right_magnitude(usize distance) const {
            bool lost_bits = false;
            Vector<u32> shifted = mag_shr(mag, distance, lost_bits);
            if (sign < 0 && lost_bits) {
                mag_increment(shifted);
            }
            return BigInteger(sign, Ops::move(shifted));
        }

        /**
         * @brief Returns the word at the given index of the infinite two's complement representation.
         * @param index The index of the word to return.
         * @param first_nonzero The index of the first non-zero word.
         * @return The word at the given index.
         */
        [[nodiscard]]
        u32 twos_word(usize index, usize first_nonzero) const noexcept {
            if (index >= mag.size()) {
                return sign < 0 ? 0xFFFFFFFFu : 0u;
            }
            if (sign >= 0) {
                return mag[index];
            }
            if (index < first_nonzero) {
                return 0;
            }
            if (index == first_nonzero) {
                return ~mag[index] + 1u;
            }
            return ~mag[index];
        }

        [[nodiscard]]
        u32 low_twos_word() const noexcept {
            return twos_word(0, first_nonzero_index(mag));
        }

        /**
         * @brief Builds a BigInteger from little-endian two's complement words.
         * @param words The little-endian two's complement words.
         * @param negative Whether the represented value is negative.
         * @return The constructed BigInteger.
         */
        [[nodiscard]]
        static BigInteger from_twos_words(Vector<u32>&& words, bool negative) {
            if (negative) {
                u64 carry = 1;
                for (u32& word: words) {
                    const u64 sum = static_cast<u64>(~word) + carry;
                    word = static_cast<u32>(sum);
                    carry = sum >> 32;
                }
                if (carry != 0) {
                    words.push_back(static_cast<u32>(carry));
                }
            }
            trim(words);
            return BigInteger(negative ? -1 : 1, Ops::move(words));
        }

        /**
         * @brief Performs a bitwise operation on two BigIntegers.
         * @tparam Op The operation to perform.
         * @param lhs The left-hand side BigInteger.
         * @param rhs The right-hand side BigInteger.
         * @param negative_result Whether the result should be negative.
         * @param operation The operation to perform.
         * @return The result of the bitwise operation.
         */
        template <typename Op>
        [[nodiscard]]
        static BigInteger bitwise_op(
            const BigInteger& lhs,
            const BigInteger& rhs,
            bool negative_result,
            Op operation
        ) {
            const usize length = Math::max(lhs.mag.size(), rhs.mag.size()) + 1;
            const usize lhs_first = first_nonzero_index(lhs.mag);
            const usize rhs_first = first_nonzero_index(rhs.mag);
            Vector<u32> words(length);
            for (usize i = 0; i < length; ++i) {
                words[i] = operation(lhs.twos_word(i, lhs_first), rhs.twos_word(i, rhs_first));
            }
            return from_twos_words(Ops::move(words), negative_result);
        }

        /**
         * @brief Returns the top `count` magnitude bits, with the lowest bit ORed with a
         * sticky bit recording whether any dropped bit was set; used for correctly
         * rounded floating-point conversion.
         * @param count The number of bits to return, must be at least 1.
         * @return The top count magnitude bits with a sticky low bit.
         */
        [[nodiscard]]
        u64 top_bits_sticky(u32 count) const noexcept {
            const usize bits = mag_bit_length(mag);
            const usize shift = bits - count;
            const usize limb_offset = shift / 32;
            const u32 bit_offset = static_cast<u32>(shift % 32);
            const auto word = [this](usize index) -> u64 {
                return index < mag.size() ? mag[index] : 0u;
            };
            u64 top = (word(limb_offset) | (word(limb_offset + 1) << 32)) >> bit_offset;
            if (bit_offset != 0) {
                top |= word(limb_offset + 2) << (64 - bit_offset);
            }
            bool sticky = false;
            for (usize i = 0; i < limb_offset && !sticky; ++i) {
                sticky = mag[i] != 0;
            }
            if (!sticky && bit_offset != 0) {
                sticky = (mag[limb_offset] & ((1u << bit_offset) - 1)) != 0;
            }
            if (sticky) {
                top |= 1u;
            }
            return top;
        }

        [[nodiscard]]
        static i32 digit_value(char c) noexcept {
            if (c >= '0' && c <= '9') {
                return c - '0';
            }
            if (c >= 'a' && c <= 'z') {
                return c - 'a' + 10;
            }
            if (c >= 'A' && c <= 'Z') {
                return c - 'A' + 10;
            }
            return -1;
        }

        /**
         * @brief Returns the largest power of radix fitting in a u32 limb and how many
         * digits it spans.
         * @param radix The radix, in the range [2, 36].
         * @param limb_radix Output parameter for the returned power of radix.
         * @return The number of digits spanned by limb_radix.
         */
        [[nodiscard]]
        static u32 digits_per_limb(u32 radix, u32& limb_radix) noexcept {
            u32 count = 1;
            u64 power = radix;
            while (power * radix <= 0xFFFFFFFFull) {
                power *= radix;
                ++count;
            }
            limb_radix = static_cast<u32>(power);
            return count;
        }
    public:
        /**
         * @brief Constructs a BigInteger with the value zero.
         */
        constexpr BigInteger() noexcept = default;

        /**
         * @brief Constructs a BigInteger from a built-in integral value.
         * @tparam T The type of the value, must be an integral type.
         * @param value The value to construct from.
         */
        template <Integral T>
        BigInteger(T value) {
            u64 magnitude = 0;
            if constexpr (SignedIntegral<T>) {
                if (value < 0) {
                    sign = -1;
                    magnitude = 0ull - static_cast<u64>(static_cast<i64>(value));
                } else if (value > 0) {
                    sign = 1;
                    magnitude = static_cast<u64>(value);
                }
            } else {
                if (value != 0) {
                    sign = 1;
                    magnitude = static_cast<u64>(value);
                }
            }
            if (magnitude != 0) {
                mag.push_back(static_cast<u32>(magnitude));
                if ((magnitude >> 32) != 0) {
                    mag.push_back(static_cast<u32>(magnitude >> 32));
                }
            }
        }

        static const BigInteger ZERO; ///< The BigInteger constant zero.
        static const BigInteger ONE; ///< The BigInteger constant one.
        static const BigInteger TWO; ///< The BigInteger constant two.
        static const BigInteger TEN; ///< The BigInteger constant ten.

        /**
         * @brief Constructs a BigInteger from the decimal string representation.
         * @param value The string representation.
         * @throws NumberFormatException If the string is not a valid representation of a BigInteger
         *
         * The string consists of an optional '+' or '-' followed by one or more
         * decimal digits.
         */
        THROWS(NumberFormatException)
        explicit BigInteger(StringView value):
            BigInteger(value, 10) {}

        /**
         * @brief Constructs a BigInteger from a string representation in the given radix.
         * @param value The string representation.
         * @param radix The radix, in the range [2, 36].
         * @throws NumberFormatException If the string is not a valid representation of a BigInteger in the given radix.
         */
        THROWS(NumberFormatException)
        BigInteger(StringView value, u32 radix) {
            if (radix < 2 || radix > 36) {
                throw NumberFormatException("Radix out of range.");
            }
            if (value.empty()) {
                throw NumberFormatException("Zero-length BigInteger.");
            }
            bool negative = false;
            usize position = 0;
            if (value[0] == '+' || value[0] == '-') {
                negative = value[0] == '-';
                position = 1;
            }
            if (position == value.size()) {
                throw NumberFormatException("No digits in BigInteger.");
            }
            u32 limb_radix = 0;
            const u32 digits_per_group = digits_per_limb(radix, limb_radix);
            const usize digit_count = value.size() - position;
            usize group_size = digit_count % digits_per_group;
            if (group_size == 0) {
                group_size = digits_per_group;
            }
            while (position < value.size()) {
                u32 group = 0;
                u32 group_radix = 1;
                for (usize i = 0; i < group_size; ++i, ++position) {
                    const i32 digit = digit_value(value[position]);
                    if (digit < 0 || static_cast<u32>(digit) >= radix) {
                        throw NumberFormatException("Illegal digit.");
                    }
                    group = group * radix + static_cast<u32>(digit);
                    group_radix *= radix;
                }
                mag_mul_add(mag, group_radix, group);
                group_size = digits_per_group;
            }
            trim(mag);
            sign = mag.empty() ? 0 : (negative ? -1 : 1);
        }

        /**
         * @brief Constructs a BigInteger from a big-endian two's complement byte array.
         * @param bytes The big-endian two's complement bytes.
         * @throws NumberFormatException If the byte array is empty.
         */
        THROWS(NumberFormatException)
        explicit BigInteger(const Vector<u8>& bytes) {
            if (bytes.empty()) {
                throw NumberFormatException("Zero-length BigInteger.");
            }
            const bool negative = (bytes[0] & 0x80u) != 0;
            const usize limb_count = (bytes.size() + 3) / 4;
            Vector<u32> words(limb_count, negative ? 0xFFFFFFFFu : 0u);
            for (usize i = 0; i < bytes.size(); ++i) {
                const u32 byte_value = bytes[bytes.size() - 1 - i];
                const u32 shift = static_cast<u32>((i % 4) * 8);
                words[i / 4] = (words[i / 4] & ~(0xFFu << shift)) | (byte_value << shift);
            }
            *this = from_twos_words(Ops::move(words), negative);
        }

        /**
         * @brief Constructs a BigInteger from a signum and a big-endian magnitude byte array.
         * @param signum_value The signum: -1 for negative, 0 for zero, 1 for positive.
         * @param magnitude The big-endian magnitude (leading zero bytes allowed).
         * @throws NumberFormatException If signum_value is out of range or mismatches the magnitude.
         */
        THROWS(NumberFormatException)
        BigInteger(i32 signum_value, const Vector<u8>& magnitude) {
            if (signum_value < -1 || signum_value > 1) {
                throw NumberFormatException("Invalid signum value.");
            }
            const usize limb_count = (magnitude.size() + 3) / 4;
            Vector<u32> words(limb_count, 0u);
            for (usize i = 0; i < magnitude.size(); ++i) {
                const u32 byte_value = magnitude[magnitude.size() - 1 - i];
                words[i / 4] |= byte_value << ((i % 4) * 8);
            }
            trim(words);
            if (!words.empty() && signum_value == 0) {
                throw NumberFormatException("Signum-magnitude mismatch.");
            }
            sign = words.empty() ? 0 : signum_value;
            mag = Ops::move(words);
        }

        /**
         * @brief Returns a BigInteger whose value is equal to that of the given i64.
         * @param value The value to convert.
         * @return A BigInteger with the same value as the given i64.
         */
        [[nodiscard]]
        static BigInteger value_of(i64 value) {
            return BigInteger(value);
        }

        /**
         * @brief Parses a BigInteger from a string in the given radix.
         * @param value The string representation.
         * @param radix The radix to use for parsing.
         * @return A BigInteger with the parsed value.
         * @throws NumberFormatException If the string is not a valid representation of a BigInteger in the given radix.
         */
        [[nodiscard]]
        THROWS(NumberFormatException)
        static BigInteger parse(StringView value, u32 radix = 10) {
            return BigInteger(value, radix);
        }

        /**
         * @brief Parses a BigInteger from a string, returning nullopt on failure.
         * @param value The string representation.
         * @param radix The radix to use for parsing.
         * @return An Optional containing the parsed BigInteger, or nullopt if parsing failed.
         */
        [[nodiscard]]
        static Optional<BigInteger> try_parse(StringView value, u32 radix = 10) noexcept {
            try {
                return BigInteger(value, radix);
            } catch (const NumberFormatException& _) {
                return nullopt;
            }
        }

        /**
         * @brief Returns the pair (this / other, this % other).
         * @param other The divisor.
         * @return The pair (quotient, remainder).
         * @throws ArithmeticException If other is zero.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        Pair<BigInteger, BigInteger> divide_and_remainder(const BigInteger& other) const {
            if (other.sign == 0) {
                throw ArithmeticException("Division by zero.");
            }
            if (sign == 0) {
                return {ZERO, ZERO};
            }
            Vector<u32> quotient;
            Vector<u32> rem;
            mag_divrem(mag, other.mag, quotient, rem);
            return {
                BigInteger(sign == other.sign ? 1 : -1, Ops::move(quotient)),
                BigInteger(sign, Ops::move(rem))
            };
        }

        /**
         * @brief Returns a BigInteger whose value is (this mod modulus), always non-negative.
         * @param modulus The modulus, which must be positive.
         * @return The non-negative residue of this modulo modulus.
         * @throws ArithmeticException If modulus is not positive.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger mod(const BigInteger& modulus) const {
            if (modulus.sign <= 0) {
                throw ArithmeticException("Modulus not positive.");
            }
            const BigInteger result = *this % modulus;
            return result.sign < 0 ? result + modulus : result;
        }

        /**
         * @brief Returns a BigInteger whose value is (this ^ exponent).
         * @param exponent The non-negative exponent.
         * @return The value of this raised to the given power.
         * @throws ArithmeticException If exponent is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger pow(i32 exponent) const {
            if (exponent < 0) {
                throw ArithmeticException("Negative exponent.");
            }
            if (exponent == 0) {
                return ONE;
            }
            if (sign == 0) {
                return ZERO;
            }
            BigInteger base = *this;
            BigInteger result = ONE;
            u32 remaining = static_cast<u32>(exponent);
            while (remaining != 0) {
                if ((remaining & 1u) != 0) {
                    result = result * base;
                }
                remaining >>= 1;
                if (remaining != 0) {
                    base = base * base;
                }
            }
            return result;
        }

        /**
         * @brief Returns the integer square root of this BigInteger (floor of the exact root).
         * @return The floor of the exact square root.
         * @throws ArithmeticException If this BigInteger is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger sqrt() const {
            if (sign < 0) {
                throw ArithmeticException("Negative BigInteger.");
            }
            if (sign == 0) {
                return ZERO;
            }
            BigInteger guess = ONE << static_cast<i32>(mag_bit_length(mag) / 2 + 1);
            while (true) {
                BigInteger next = (guess + *this / guess) >> 1;
                if (next >= guess) {
                    return guess;
                }
                guess = Ops::move(next);
            }
        }

        /**
         * @brief Returns the pair (sqrt(this), this - sqrt(this)^2).
         * @return The pair (integer square root, remainder).
         * @throws ArithmeticException If this BigInteger is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        Pair<BigInteger, BigInteger> sqrt_and_remainder() const {
            const BigInteger root = sqrt();
            return {root, *this - root * root};
        }

        /**
         * @brief Returns the greatest common divisor of abs(this) and abs(other).
         * @param other The other operand.
         * @return The greatest common divisor of abs(this) and abs(other).
         */
        [[nodiscard]]
        BigInteger gcd(const BigInteger& other) const {
            BigInteger a = abs();
            BigInteger b = other.abs();
            while (b.sign != 0) {
                BigInteger r = a % b;
                a = Ops::move(b);
                b = Ops::move(r);
            }
            return a;
        }

        /**
         * @brief Returns the least common multiple of abs(this) and abs(other), or zero
         * if either is zero.
         * @param other The other operand.
         * @return The least common multiple of abs(this) and abs(other), or zero if either is zero.
         */
        [[nodiscard]]
        BigInteger lcm(const BigInteger& other) const {
            if (sign == 0 || other.sign == 0) {
                return ZERO;
            }
            return (*this / gcd(other) * other).abs();
        }

        /**
         * @brief Returns a BigInteger whose value is the absolute value of this.
         * @return The absolute value of this BigInteger.
         */
        [[nodiscard]]
        BigInteger abs() const {
            BigInteger result = *this;
            result.sign = sign < 0 ? 1 : sign;
            return result;
        }

        /**
         * @brief Returns the signum of this BigInteger: -1, 0 or 1.
         * @return The signum: -1, 0 or 1.
         */
        [[nodiscard]]
        i32 signum() const noexcept {
            return sign;
        }

        /**
         * @brief Returns (this ^ exponent mod modulus), supporting negative exponents
         * when this is invertible modulo modulus.
         * @param exponent The exponent.
         * @param modulus The modulus, which must be positive.
         * @return A BigInteger whose value is (this ^ exponent mod modulus).
         * @throws ArithmeticException if modulus is not positive, if exponent is negative and this is
         * not invertible modulo modulus, or if this is zero and exponent is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger mod_pow(const BigInteger& exponent, const BigInteger& modulus) const {
            if (modulus.sign <= 0) {
                throw ArithmeticException("Modulus not positive.");
            }
            if (exponent.sign < 0) {
                return mod_inverse(modulus).mod_pow(-exponent, modulus);
            }
            if (modulus == ONE) {
                return ZERO;
            }
            BigInteger base = mod(modulus);
            BigInteger result = ONE;
            const i32 bits = exponent.bit_length();
            for (i32 i = 0; i < bits; ++i) {
                if (exponent.test_bit(i)) {
                    result = result * base % modulus;
                }
                if (i + 1 < bits) {
                    base = base * base % modulus;
                }
            }
            return result;
        }

        /**
         * @brief Returns (this ^ -1 mod modulus).
         * @param modulus The modulus, which must be positive.
         * @return A BigInteger whose value is the modular inverse of this modulo modulus.
         * @throws ArithmeticException if modulus is not positive or if this is not invertible modulo modulus.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger mod_inverse(const BigInteger& modulus) const {
            if (modulus.sign <= 0) {
                throw ArithmeticException("Modulus not positive.");
            }
            if (modulus == ONE) {
                return ZERO;
            }
            BigInteger previous_r = mod(modulus);
            BigInteger current_r = modulus;
            BigInteger previous_s = ONE;
            BigInteger current_s = ZERO;
            if (previous_r.sign == 0) {
                throw ArithmeticException("BigInteger not invertible.");
            }
            while (current_r.sign != 0) {
                auto [quotient, rem] = previous_r.divide_and_remainder(current_r);
                BigInteger next_s = previous_s - quotient * current_s;
                previous_r = Ops::move(current_r);
                current_r = Ops::move(rem);
                previous_s = Ops::move(current_s);
                current_s = Ops::move(next_s);
            }
            if (previous_r != ONE) {
                throw ArithmeticException("BigInteger not invertible.");
            }
            return previous_s.mod(modulus);
        }

        /**
         * @brief Returns (this & ~other), clearing in this every bit set in other.
         * @param other The BigInteger whose set bits will be cleared in the result.
         * @return A BigInteger with the same bits as this, except that any bit set in other is cleared.
         */
        [[nodiscard]]
        BigInteger and_not(const BigInteger& other) const {
            return bitwise_op(
                *this,
                other,
                sign < 0 && other.sign >= 0,
                [](u32 a, u32 b) -> u32 { return a & ~b; }
            );
        }

        /**
         * @brief Tests whether the bit at the given index of the two's complement
         * representation is set.
         * @param index The bit index, starting at 0 for the least significant bit.
         * @return True if the bit at the given index is set, false otherwise.
         * @throws ArithmeticException if index is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        bool test_bit(i32 index) const {
            if (index < 0) {
                throw ArithmeticException("Negative bit address.");
            }
            const usize fnz = first_nonzero_index(mag);
            const u32 word = twos_word(static_cast<usize>(index) / 32, fnz);
            return ((word >> (static_cast<u32>(index) % 32)) & 1u) != 0;
        }

        /**
         * @brief Returns a BigInteger with the designated bit set.
         * @param index The bit index, starting at 0 for the least significant bit.
         * @return A BigInteger with the designated bit set.
         * @throws ArithmeticException if index is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger set_bit(i32 index) const {
            if (index < 0) {
                throw ArithmeticException("Negative bit address.");
            }
            return *this | (ONE << index);
        }

        /**
         * @brief Returns a BigInteger with the designated bit cleared.
         * @param index The bit index, starting at 0 for the least significant bit.
         * @return A BigInteger with the designated bit cleared.
         * @throws ArithmeticException if index is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger clear_bit(i32 index) const {
            if (index < 0) {
                throw ArithmeticException("Negative bit address.");
            }
            return and_not(ONE << index);
        }

        /**
         * @brief Returns a BigInteger with the designated bit flipped.
         * @param index The bit index, starting at 0 for the least significant bit.
         * @return A BigInteger with the designated bit flipped.
         * @throws ArithmeticException if index is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger flip_bit(i32 index) const {
            if (index < 0) {
                throw ArithmeticException("Negative bit address.");
            }
            return *this ^ (ONE << index);
        }

        /**
         * @brief Returns the number of bits in the minimal two's complement
         * representation of this BigInteger, excluding the sign bit.
         * @return The number of bits in the minimal two's complement representation of this BigInteger, excluding the sign bit.
         */
        [[nodiscard]]
        i32 bit_length() const noexcept {
            if (sign == 0) {
                return 0;
            }
            usize bits = mag_bit_length(mag);
            if (sign < 0 && mag_is_power_of_two(mag)) {
                --bits;
            }
            return static_cast<i32>(bits);
        }

        /**
         * @brief Returns the number of bits in the two's complement representation
         * that differ from the sign bit.
         * @return The number of bits in the two's complement representation that differ from the sign bit.
         */
        [[nodiscard]]
        i32 bit_count() const noexcept {
            usize count = 0;
            for (u32 limb: mag) {
                count += static_cast<usize>(Math::count_pop(limb));
            }
            if (sign < 0) {
                const usize fnz = first_nonzero_index(mag);
                count += fnz * 32 + static_cast<usize>(Math::count_right_zero(mag[fnz])) - 1;
            }
            return static_cast<i32>(count);
        }

        /**
         * @brief Returns the index of the rightmost set bit, or -1 if this is zero.
         * @return The index of the rightmost set bit, or -1 if this is zero.
         */
        [[nodiscard]]
        i32 get_lowest_set_bit() const noexcept {
            if (sign == 0) {
                return -1;
            }
            const usize fnz = first_nonzero_index(mag);
            return static_cast<i32>(fnz * 32 + static_cast<usize>(Math::count_right_zero(mag[fnz])));
        }

        /**
         * @brief Returns true if this BigInteger is probably prime, false if it is
         * definitely composite.
         * @param certainty The threshold controlling the number of Miller-Rabin rounds.
         * @return True if this is probably prime, false if definitely composite.
         *
         * Uses trial division followed by deterministic-base Miller-Rabin rounds.
         * A certainty of zero or less always yields true.
         */
        [[nodiscard]]
        bool is_probable_prime(i32 certainty = 100) const {
            static constexpr Array<u32, 24> SMALL_PRIMES = {
                3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
                43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
            };
            static constexpr Array<u32, 20> BASES = {
                2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                31, 37, 41, 43, 47, 53, 59, 61, 67, 71
            };
            if (certainty <= 0) {
                return true;
            }
            const BigInteger w = abs();
            if (w == TWO) {
                return true;
            }
            if (w.sign == 0 || w == ONE || !w.test_bit(0)) {
                return false;
            }
            for (u32 prime: SMALL_PRIMES) {
                const BigInteger value(prime);
                if (w == value) {
                    return true;
                }
                if ((w % value).sign == 0) {
                    return false;
                }
            }
            const BigInteger w_minus_one = w - ONE;
            const i32 shift = w_minus_one.get_lowest_set_bit();
            const BigInteger odd_part = w_minus_one >> shift;
            const usize rounds = Math::min(
                Ops::size(BASES),
                static_cast<usize>(Math::max(1, (certainty + 1) / 2))
            );
            for (usize i = 0; i < rounds; ++i) {
                BigInteger x = BigInteger(BASES[i]).mod_pow(odd_part, w);
                if (x == ONE || x == w_minus_one) {
                    continue;
                }
                bool composite = true;
                for (i32 r = 1; r < shift; ++r) {
                    x = x * x % w;
                    if (x == w_minus_one) {
                        composite = false;
                        break;
                    }
                    if (x == ONE) {
                        break;
                    }
                }
                if (composite) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Returns the first probable prime greater than this BigInteger.
         * @return The next probable prime greater than this BigInteger.
         * @throws ArithmeticException if this BigInteger is negative.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        BigInteger next_probable_prime() const {
            if (sign < 0) {
                throw ArithmeticException("Negative BigInteger.");
            }
            if (*this < TWO) {
                return TWO;
            }
            BigInteger candidate = *this + ONE;
            if (!candidate.test_bit(0)) {
                candidate += ONE;
            }
            while (!candidate.is_probable_prime()) {
                candidate += TWO;
            }
            return candidate;
        }

        [[nodiscard]]
        bool operator==(const BigInteger&) const noexcept = default;

        [[nodiscard]]
        StrongOrdering operator<=>(const BigInteger& other) const noexcept {
            if (sign != other.sign) {
                return sign <=> other.sign;
            }
            const i32 comparison = sign < 0
                ? mag_cmp(other.mag, mag)
                : mag_cmp(mag, other.mag);
            if (comparison < 0) {
                return StrongOrdering::LESS;
            }
            return comparison > 0 ? StrongOrdering::GREATER : StrongOrdering::EQUAL;
        }

        /**
         * @brief Converts this BigInteger to an i8, keeping only the low-order 8 bits.
         * @return The value of this BigInteger as an i8, truncated to the low-order 8 bits.
         */
        [[nodiscard]]
        i8 byte_value() const noexcept {
            return static_cast<i8>(static_cast<u8>(low_twos_word() & 0xFFu));
        }

        /**
         * @brief Converts this BigInteger to an i16, keeping only the low-order 16 bits.
         * @return The value of this BigInteger as an i16, truncated to the low-order 16 bits.
         */
        [[nodiscard]]
        i16 short_value() const noexcept {
            return static_cast<i16>(static_cast<u16>(low_twos_word() & 0xFFFFu));
        }

        /**
         * @brief Converts this BigInteger to an i32, keeping only the low-order 32 bits.
         * @return The value of this BigInteger as an i32, truncated to the low-order 32 bits.
         */
        [[nodiscard]]
        i32 int_value() const noexcept {
            return static_cast<i32>(low_twos_word());
        }

        /**
         * @brief Converts this BigInteger to an i64, keeping only the low-order 64 bits.
         * @return The value of this BigInteger as an i64, truncated to the low-order 64 bits.
         */
        [[nodiscard]]
        i64 long_value() const noexcept {
            const usize fnz = first_nonzero_index(mag);
            const u64 low = static_cast<u64>(twos_word(0, fnz))
                | (static_cast<u64>(twos_word(1, fnz)) << 32);
            return static_cast<i64>(low);
        }

        /**
         * @brief Converts this BigInteger to an i8.
         * @return The value of this BigInteger as an i8.
         * @throws ArithmeticException if this BigInteger does not fit in an i8.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        i8 byte_value_exact() const {
            if (bit_length() > 7) {
                throw ArithmeticException("BigInteger out of byte range.");
            }
            return byte_value();
        }

        /**
         * @brief Converts this BigInteger to an i16.
         * @return The value of this BigInteger as an i16.
         * @throws ArithmeticException if this BigInteger does not fit in an i16.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        i16 short_value_exact() const {
            if (bit_length() > 15) {
                throw ArithmeticException("BigInteger out of short range.");
            }
            return short_value();
        }

        /**
         * @brief Converts this BigInteger to an i32.
         * @return The value of this BigInteger as an i32.
         * @throws ArithmeticException if this BigInteger does not fit in an i32.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        i32 int_value_exact() const {
            if (bit_length() > 31) {
                throw ArithmeticException("BigInteger out of int range.");
            }
            return int_value();
        }

        /**
         * @brief Converts this BigInteger to an i64, throwing if the value does not fit.
         * @return The value of this BigInteger as an i64.
         * @throws ArithmeticException If this BigInteger does not fit in an i64.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        i64 long_value_exact() const {
            if (bit_length() > 63) {
                throw ArithmeticException("BigInteger out of long range.");
            }
            return long_value();
        }

        /**
         * @brief Converts this BigInteger to an f32, rounding to nearest.
         * @return The value of this BigInteger as an f32.
         *
         * Values too large in magnitude convert to the appropriately signed infinity.
         */
        [[nodiscard]]
        f32 float_value() const noexcept {
            if (sign == 0) {
                return 0.0f;
            }
            const usize bits = mag_bit_length(mag);
            f32 magnitude;
            if (bits <= 31) {
                magnitude = static_cast<f32>(mag[0]);
            } else if (bits > 256) {
                magnitude = Float::POSITIVE_INFINITY;
            } else {
                magnitude = Math::ldexp(
                    static_cast<f32>(top_bits_sticky(31)),
                    static_cast<i32>(bits - 31)
                );
            }
            return sign < 0 ? -magnitude : magnitude;
        }

        /**
         * @brief Converts this BigInteger to an f64, rounding to nearest.
         * @return The value of this BigInteger as an f64.
         *
         * Values too large in magnitude convert to the appropriately signed infinity.
         */
        [[nodiscard]]
        f64 double_value() const noexcept {
            if (sign == 0) {
                return 0.0;
            }
            const usize bits = mag_bit_length(mag);
            f64 magnitude;
            if (bits <= 63) {
                u64 value = mag[0];
                if (mag.size() > 1) {
                    value |= static_cast<u64>(mag[1]) << 32;
                }
                magnitude = static_cast<f64>(value);
            } else if (bits > 2048) {
                magnitude = Double::POSITIVE_INFINITY;
            } else {
                magnitude = Math::ldexp(
                    static_cast<f64>(top_bits_sticky(63)),
                    static_cast<i32>(bits - 63)
                );
            }
            return sign < 0 ? -magnitude : magnitude;
        }

        /**
         * @brief Returns the string representation of this BigInteger in the given radix.
         * @param radix The radix, in the range [2, 36].
         * @return The string representation in the given radix.
         *
         * A radix outside [2, 36] falls back to radix 10. Digits ten
         * through thirty-five are the lowercase letters 'a' through 'z'.
         */
        [[nodiscard]]
        String to_string(u32 radix = 10) const {
            static constexpr StringView DIGIT_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz";
            if (radix < 2 || radix > 36) {
                radix = 10;
            }
            if (sign == 0) {
                return "0";
            }
            u32 limb_radix = 0;
            const u32 digits_per_group = digits_per_limb(radix, limb_radix);
            Vector<u32> groups;
            Vector<u32> work = mag;
            while (!work.empty()) {
                groups.push_back(mag_divrem_word(work, limb_radix));
            }
            String result;
            result.reserve(groups.size() * digits_per_group + 1);
            if (sign < 0) {
                result += '-';
            }
            String group_digits;
            for (usize i = groups.size(); i-- > 0;) {
                group_digits.clear();
                u32 group = groups[i];
                while (group != 0) {
                    group_digits += DIGIT_CHARS[group % radix];
                    group /= radix;
                }
                if (i + 1 != groups.size()) {
                    while (group_digits.size() < digits_per_group) {
                        group_digits += '0';
                    }
                }
                for (usize k = group_digits.size(); k-- > 0;) {
                    result += group_digits[k];
                }
            }
            return result;
        }

        /**
         * @brief Returns the big-endian two's complement byte representation of this
         * BigInteger, using the minimal number of bytes.
         * @return The minimal big-endian two's complement byte array.
         */
        [[nodiscard]]
        Vector<u8> to_byte_array() const {
            const usize byte_count = static_cast<usize>(bit_length()) / 8 + 1;
            Vector<u8> bytes(byte_count);
            const usize fnz = first_nonzero_index(mag);
            for (usize i = 0; i < byte_count; ++i) {
                const u32 word = twos_word(i / 4, fnz);
                bytes[byte_count - 1 - i] = static_cast<u8>(word >> ((i % 4) * 8));
            }
            return bytes;
        }

        /**
         * @brief Returns a BigInteger whose value is (lhs + rhs).
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The sum (lhs + rhs).
         */
        [[nodiscard]]
        friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
            if (lhs.sign == 0) {
                return rhs;
            }
            if (rhs.sign == 0) {
                return lhs;
            }
            if (lhs.sign == rhs.sign) {
                return BigInteger(lhs.sign, mag_add(lhs.mag, rhs.mag));
            }
            const i32 comparison = mag_cmp(lhs.mag, rhs.mag);
            if (comparison == 0) {
                return ZERO;
            }
            return comparison > 0
                ? BigInteger(lhs.sign, mag_sub(lhs.mag, rhs.mag))
                : BigInteger(rhs.sign, mag_sub(rhs.mag, lhs.mag));
        }

        /**
         * @brief Returns a BigInteger whose value is (lhs - rhs).
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The difference (lhs - rhs).
         */
        [[nodiscard]]
        friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
            if (rhs.sign == 0) {
                return lhs;
            }
            if (lhs.sign == 0) {
                return -rhs;
            }
            if (lhs.sign != rhs.sign) {
                return BigInteger(lhs.sign, mag_add(lhs.mag, rhs.mag));
            }
            const i32 comparison = mag_cmp(lhs.mag, rhs.mag);
            if (comparison == 0) {
                return ZERO;
            }
            return comparison > 0
                ? BigInteger(lhs.sign, mag_sub(lhs.mag, rhs.mag))
                : BigInteger(-lhs.sign, mag_sub(rhs.mag, lhs.mag));
        }

        /**
         * @brief Returns a BigInteger whose value is (lhs * rhs).
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The product (lhs * rhs).
         */
        [[nodiscard]]
        friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
            if (lhs.sign == 0 || rhs.sign == 0) {
                return ZERO;
            }
            return BigInteger(lhs.sign == rhs.sign ? 1 : -1, mag_mul(lhs.mag, rhs.mag));
        }

        /**
         * @brief Returns a BigInteger whose value is (lhs / rhs), truncated towards zero.
         * @param lhs The dividend.
         * @param rhs The divisor.
         * @return The quotient (lhs / rhs), truncated towards zero.
         * @throws ArithmeticException If rhs is zero.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
            return lhs.divide_and_remainder(rhs).first;
        }

        /**
         * @brief Returns a BigInteger whose value is (lhs % rhs), with the sign of lhs.
         * @param lhs The dividend.
         * @param rhs The divisor.
         * @return The remainder (lhs % rhs), with the sign of lhs.
         * @throws ArithmeticException If rhs is zero.
         */
        [[nodiscard]]
        THROWS(ArithmeticException)
        friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
            return lhs.divide_and_remainder(rhs).second;
        }

        /**
         * @brief Bitwise AND over the two's complement representations.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The bitwise AND (lhs & rhs).
         */
        [[nodiscard]]
        friend BigInteger operator&(const BigInteger& lhs, const BigInteger& rhs) {
            return bitwise_op(
                lhs,
                rhs,
                lhs.sign < 0 && rhs.sign < 0,
                [](u32 a, u32 b) -> u32 { return a & b; }
            );
        }

        /**
         * @brief Bitwise OR over the two's complement representations.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The bitwise OR (lhs | rhs).
         */
        [[nodiscard]]
        friend BigInteger operator|(const BigInteger& lhs, const BigInteger& rhs) {
            return bitwise_op(
                lhs,
                rhs,
                lhs.sign < 0 || rhs.sign < 0,
                [](u32 a, u32 b) -> u32 { return a | b; }
            );
        }

        /**
         * @brief Bitwise XOR over the two's complement representations.
         * @param lhs The left-hand operand.
         * @param rhs The right-hand operand.
         * @return The bitwise XOR (lhs ^ rhs).
         */
        [[nodiscard]]
        friend BigInteger operator^(const BigInteger& lhs, const BigInteger& rhs) {
            return bitwise_op(
                lhs,
                rhs,
                (lhs.sign < 0) != (rhs.sign < 0),
                [](u32 a, u32 b) -> u32 { return a ^ b; }
            );
        }

        /**
         * @brief Bitwise complement, (-this - 1).
         * @return The bitwise complement of this BigInteger.
         */
        [[nodiscard]]
        BigInteger operator~() const {
            return -(*this) - ONE;
        }

        /**
         * @brief Returns a BigInteger whose value is (-this).
         * @return The negation of this BigInteger.
         */
        [[nodiscard]]
        BigInteger operator-() const {
            BigInteger result = *this;
            result.sign = -result.sign;
            return result;
        }

        [[nodiscard]]
        BigInteger operator+() const {
            return *this;
        }

        /**
         * @brief Returns a BigInteger whose value is (this << distance).
         * @param distance The shift distance; a negative distance shifts right instead.
         * @return The left-shifted value.
         */
        [[nodiscard]]
        BigInteger operator<<(i32 distance) const {
            if (sign == 0 || distance == 0) {
                return *this;
            }
            return distance > 0
                ? BigInteger(sign, mag_shl(mag, static_cast<usize>(distance)))
                : shift_right_magnitude(static_cast<usize>(0ull - static_cast<u64>(static_cast<i64>(distance))));
        }

        /**
         * @brief Returns a BigInteger whose value is (this >> distance), an arithmetic
         * shift rounding towards negative infinity.
         * @param distance The shift distance; a negative distance shifts left instead.
         * @return The right-shifted value.
         */
        [[nodiscard]]
        BigInteger operator>>(i32 distance) const {
            if (sign == 0 || distance == 0) {
                return *this;
            }
            return distance > 0
                ? shift_right_magnitude(static_cast<usize>(distance))
                : BigInteger(
                    sign,
                    mag_shl(mag, static_cast<usize>(0ull - static_cast<u64>(static_cast<i64>(distance))))
                );
        }

        BigInteger& operator+=(const BigInteger& other) {
            *this = *this + other;
            return *this;
        }

        BigInteger& operator-=(const BigInteger& other) {
            *this = *this - other;
            return *this;
        }

        BigInteger& operator*=(const BigInteger& other) {
            *this = *this * other;
            return *this;
        }

        THROWS(ArithmeticException)
        BigInteger& operator/=(const BigInteger& other) {
            *this = *this / other;
            return *this;
        }

        THROWS(ArithmeticException)
        BigInteger& operator%=(const BigInteger& other) {
            *this = *this % other;
            return *this;
        }

        BigInteger& operator&=(const BigInteger& other) {
            *this = *this & other;
            return *this;
        }

        BigInteger& operator|=(const BigInteger& other) {
            *this = *this | other;
            return *this;
        }

        BigInteger& operator^=(const BigInteger& other) {
            *this = *this ^ other;
            return *this;
        }

        BigInteger& operator<<=(i32 distance) {
            *this = *this << distance;
            return *this;
        }

        BigInteger& operator>>=(i32 distance) {
            *this = *this >> distance;
            return *this;
        }

        BigInteger& operator++() {
            *this = *this + ONE;
            return *this;
        }

        BigInteger operator++(int) {
            BigInteger previous = *this;
            *this = *this + ONE;
            return previous;
        }

        BigInteger& operator--() {
            *this = *this - ONE;
            return *this;
        }

        BigInteger operator--(int) {
            BigInteger previous = *this;
            *this = *this - ONE;
            return previous;
        }
    };

    inline const BigInteger BigInteger::ZERO{0};
    inline const BigInteger BigInteger::ONE{1};
    inline const BigInteger BigInteger::TWO{2};
    inline const BigInteger BigInteger::TEN{10};

    /**
     * @brief Returns a BigInteger whose value is the absolute value of the argument.
     * @param value The BigInteger to take the absolute value of.
     * @return The absolute value of the argument.
     */
    [[nodiscard]]
    inline BigInteger abs(const BigInteger& value) {
        return value.abs();
    }

    /**
     * @brief Returns the smaller of two BigInteger values.
     * @param a The first value.
     * @param b The second value.
     * @return The smaller of a and b.
     */
    [[nodiscard]]
    inline const BigInteger& min(const BigInteger& a, const BigInteger& b) {
        return b < a ? b : a;
    }

    /**
     * @brief Returns the larger of two BigInteger values.
     * @param a The first value.
     * @param b The second value.
     * @return The larger of a and b.
     */
    [[nodiscard]]
    inline const BigInteger& max(const BigInteger& a, const BigInteger& b) {
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
    inline const BigInteger& clamp(const BigInteger& value, const BigInteger& low, const BigInteger& high) {
        return value < low ? low : (high < value ? high : value);
    }

    /**
     * @brief Returns the signum of the BigInteger: -1, 0 or 1.
     * @param value The BigInteger to inspect.
     * @return The signum: -1, 0 or 1.
     */
    [[nodiscard]]
    inline i32 signum(const BigInteger& value) noexcept {
        return value.signum();
    }

    /**
     * @brief Returns the greatest common divisor of abs(a) and abs(b).
     * @param a The first operand.
     * @param b The second operand.
     * @return The greatest common divisor of abs(a) and abs(b).
     */
    [[nodiscard]]
    inline BigInteger gcd(const BigInteger& a, const BigInteger& b) {
        return a.gcd(b);
    }

    /**
     * @brief Returns the least common multiple of abs(a) and abs(b), or zero if either is zero.
     * @param a The first operand.
     * @param b The second operand.
     * @return The least common multiple of abs(a) and abs(b), or zero if either is zero.
     */
    [[nodiscard]]
    inline BigInteger lcm(const BigInteger& a, const BigInteger& b) {
        return a.lcm(b);
    }

    /**
     * @brief Returns base raised to the given non-negative exponent.
     * @param base The base.
     * @param exponent The non-negative exponent.
     * @return The value of base raised to the given power.
     * @throws ArithmeticException If exponent is negative.
     */
    [[nodiscard]]
    THROWS(ArithmeticException)
    inline BigInteger pow(const BigInteger& base, i32 exponent) {
        return base.pow(exponent);
    }

    /**
     * @brief Returns the integer square root (floor) of value.
     * @param value The non-negative value to take the square root of.
     * @return The floor of the exact square root of value.
     * @throws ArithmeticException If value is negative.
     */
    [[nodiscard]]
    THROWS(ArithmeticException)
    inline BigInteger sqrt(const BigInteger& value) {
        return value.sqrt();
    }
}

using stdx::math::BigInteger;

namespace stdx {
    namespace core {
        template <>
        struct Hash<BigInteger> {
            [[nodiscard]]
            usize operator()(const BigInteger& value) const noexcept {
                return value.hash_code();
            }
        };
    }

    namespace fmt {
        template <>
        struct Formatter<BigInteger> {
            static constexpr const char* parse(FormatParseContext& ctx) noexcept {
                return ctx.begin();
            }

            static FormatContext::iterator format(const BigInteger& value, FormatContext& ctx) {
                return format_to(ctx.out(), "{}", value.to_string());
            }
        };
    }
}

template <>
struct stdx::core::hash<BigInteger> : public stdx::core::Hash<BigInteger> {};

template <>
struct stdx::fmt::formatter<BigInteger> : public stdx::fmt::Formatter<BigInteger> {};

export namespace stdx::literals::inline math_literals {
    [[nodiscard]]
    inline BigInteger operator""_bi(const char* s, usize len) {
        return BigInteger(StringView(s, len));
    }
}
