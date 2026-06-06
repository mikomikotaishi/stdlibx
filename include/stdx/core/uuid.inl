#pragma once

using stdx::random::MersenneTwister64;
using stdx::random::RandomDevice;
using stdx::time::SystemClock;
using stdx::time::Milliseconds;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @class Uuid
     * @brief A 128-bit universally unique identifier (RFC 9562).
     *
     * Supports version 4 (random) and version 7 (timestamp-ordered).
     * Immutable value type with comparison, hashing, and string conversion.
     */
    class [[nodiscard]] Uuid {
    public:
        static const Uuid NIL; ///< The nil UUID (all zeros).
        static const Uuid MAX; ///< The max UUID (all ones).
    private:
        Array<ByteUnit, 16> data{};

        explicit constexpr Uuid(Array<ByteUnit, 16> bytes) noexcept:
            data{bytes} {}

        static constexpr StringView HEX_DIGITS = "0123456789abcdef";

        [[nodiscard]]
        static constexpr u8 hex_value(char c) {
            if (c >= '0' && c <= '9') {
                return static_cast<u8>(c - '0');
            } else if (c >= 'a' && c <= 'f') {
                return static_cast<u8>(c - 'a' + 10);
            } else if (c >= 'A' && c <= 'F') {
                return static_cast<u8>(c - 'A' + 10);
            }
            return 0xFF;
        }

        static void set_version_and_variant(Array<ByteUnit, 16>& bytes, u8 version) noexcept {
            bytes[6] = static_cast<ByteUnit>((Byte::to_integer<u8>(bytes[6]) & 0x0F) | (version << 4));
            bytes[8] = static_cast<ByteUnit>((Byte::to_integer<u8>(bytes[8]) & 0x3F) | 0x80);
        }
    public:
        Uuid() noexcept = default;

        /**
         * @brief Constructs a UUID from two 64-bit values (most/least significant bits).
         * @param most_significant_bits The upper 64 bits.
         * @param least_significant_bits The lower 64 bits.
         */
        constexpr Uuid(u64 most_significant_bits, u64 least_significant_bits) noexcept:
            data{
                static_cast<ByteUnit>(most_significant_bits >> 56),
                static_cast<ByteUnit>(most_significant_bits >> 48),
                static_cast<ByteUnit>(most_significant_bits >> 40),
                static_cast<ByteUnit>(most_significant_bits >> 32),
                static_cast<ByteUnit>(most_significant_bits >> 24),
                static_cast<ByteUnit>(most_significant_bits >> 16),
                static_cast<ByteUnit>(most_significant_bits >> 8),
                static_cast<ByteUnit>(most_significant_bits),
                static_cast<ByteUnit>(least_significant_bits >> 56),
                static_cast<ByteUnit>(least_significant_bits >> 48),
                static_cast<ByteUnit>(least_significant_bits >> 40),
                static_cast<ByteUnit>(least_significant_bits >> 32),
                static_cast<ByteUnit>(least_significant_bits >> 24),
                static_cast<ByteUnit>(least_significant_bits >> 16),
                static_cast<ByteUnit>(least_significant_bits >> 8),
                static_cast<ByteUnit>(least_significant_bits)
            } {}

        /**
         * @brief Generates a random UUID (version 4).
         */
        [[nodiscard]]
        static Uuid random_uuid() {
            static thread_local MersenneTwister64 engine{RandomDevice{}()};
            Array<ByteUnit, 16> bytes{};

            u64 high = engine();
            u64 low = engine();
            for (usize i = 0; i < 8; ++i) {
                bytes[i] = static_cast<ByteUnit>(high >> (56 - i * 8));
                bytes[i + 8] = static_cast<ByteUnit>(low >> (56 - i * 8));
            }

            set_version_and_variant(bytes, 4);
            return Uuid(bytes);
        }

        /**
         * @brief Generates a time-ordered UUID (version 7).
         *
         * Uses a 48-bit Unix timestamp in milliseconds followed by 80 random bits,
         * with version and variant fields set per RFC 9562.
         */
        [[nodiscard]]
        static Uuid timestamp_uuid() {
            static thread_local MersenneTwister64 engine{RandomDevice{}()};

            const u64 timestamp = static_cast<u64>(
                stdx::time::duration_cast<Milliseconds>(
                    SystemClock::now().time_since_epoch()
                ).count()
            );

            Array<ByteUnit, 16> bytes{};

            // 48-bit big-endian timestamp
            bytes[0] = static_cast<ByteUnit>(timestamp >> 40);
            bytes[1] = static_cast<ByteUnit>(timestamp >> 32);
            bytes[2] = static_cast<ByteUnit>(timestamp >> 24);
            bytes[3] = static_cast<ByteUnit>(timestamp >> 16);
            bytes[4] = static_cast<ByteUnit>(timestamp >> 8);
            bytes[5] = static_cast<ByteUnit>(timestamp);

            // Random fill for remaining bytes
            u64 rand_a = engine();
            u64 rand_b = engine();
            bytes[6] = static_cast<ByteUnit>(rand_a >> 56);
            bytes[7] = static_cast<ByteUnit>(rand_a >> 48);
            bytes[8] = static_cast<ByteUnit>(rand_a >> 40);
            bytes[9] = static_cast<ByteUnit>(rand_a >> 32);
            bytes[10] = static_cast<ByteUnit>(rand_b >> 56);
            bytes[11] = static_cast<ByteUnit>(rand_b >> 48);
            bytes[12] = static_cast<ByteUnit>(rand_b >> 40);
            bytes[13] = static_cast<ByteUnit>(rand_b >> 32);
            bytes[14] = static_cast<ByteUnit>(rand_b >> 24);
            bytes[15] = static_cast<ByteUnit>(rand_b >> 16);

            set_version_and_variant(bytes, 7);
            return Uuid(bytes);
        }

        /**
         * @brief Parses a UUID from its canonical string representation.
         * @param str A string in the format "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
         * @return The parsed UUID, or an empty optional if the string is invalid.
         */
        [[nodiscard]]
        static Optional<Uuid> from_string(StringView str) noexcept {
            if (str.size() != 36) {
                return nullopt;
            }
            if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-') {
                return nullopt;
            }

            Array<ByteUnit, 16> bytes{};
            usize byte_idx = 0;

            for (usize i = 0; i < 36 && byte_idx < 16; ++i) {
                if (str[i] == '-') {
                    continue;
                }

                u8 high = hex_value(str[i]);
                if (high == 0xFF || i + 1 >= 36) {
                    return nullopt;
                }

                u8 low = hex_value(str[++i]);
                if (low == 0xFF) {
                    return nullopt;
                }

                bytes[byte_idx++] = static_cast<ByteUnit>((high << 4) | low);
            }

            if (byte_idx != 16) {
                return nullopt;
            }
            return Uuid(bytes);
        }

        /**
         * @brief Returns the canonical string representation.
         * @return A string in the format "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
         */
        [[nodiscard]]
        String to_string() const {
            String result;
            result.reserve(36);

            for (usize i = 0; i < 16; ++i) {
                if (i == 4 || i == 6 || i == 8 || i == 10) {
                    result += '-';
                }
                u8 byte = Byte::to_integer<u8>(data[i]);
                result += HEX_DIGITS[byte >> 4];
                result += HEX_DIGITS[byte & 0x0F];
            }

            return result;
        }

        /**
         * @brief Returns the UUID version (4 for random, 7 for timestamp-ordered).
         */
        [[nodiscard]]
        constexpr u8 version() const noexcept {
            return static_cast<u8>((Byte::to_integer<u8>(data[6]) >> 4) & 0x0F);
        }

        /**
         * @brief Returns the UUID variant (2 for RFC 9562 compliant).
         */
        [[nodiscard]]
        constexpr u8 variant() const noexcept {
            return static_cast<u8>((Byte::to_integer<u8>(data[8]) >> 6) & 0x03);
        }

        /**
         * @brief Returns the most significant 64 bits.
         */
        [[nodiscard]]
        constexpr u64 most_significant_bits() const noexcept {
            u64 result = 0;
            for (usize i = 0; i < 8; ++i) {
                result = (result << 8) | Byte::to_integer<u64>(data[i]);
            }
            return result;
        }

        /**
         * @brief Returns the least significant 64 bits.
         */
        [[nodiscard]]
        constexpr u64 least_significant_bits() const noexcept {
            u64 result = 0;
            for (usize i = 8; i < 16; ++i) {
                result = (result << 8) | Byte::to_integer<u64>(data[i]);
            }
            return result;
        }

        /**
         * @brief Extracts the Unix timestamp in milliseconds (valid for version 7 only).
         */
        [[nodiscard]]
        constexpr u64 timestamp() const noexcept {
            u64 result = 0;
            for (usize i = 0; i < 6; ++i) {
                result = (result << 8) | Byte::to_integer<u64>(data[i]);
            }
            return result;
        }

        /**
         * @brief Returns true if this is the nil UUID.
         */
        [[nodiscard]]
        constexpr bool is_nil() const noexcept {
            for (ByteUnit byte: data) {
                if (byte != ByteUnit{0}) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Returns a view of the raw 16 bytes.
         */
        [[nodiscard]]
        constexpr Span<const ByteUnit, 16> bytes() const noexcept {
            return Span<const ByteUnit, 16>{data};
        }

        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const Uuid& other) const noexcept = default;

        [[nodiscard]]
        constexpr bool operator==(const Uuid& other) const noexcept = default;
    };

    inline constexpr Uuid Uuid::NIL = Uuid();
    inline constexpr Uuid Uuid::MAX = Uuid(Array<ByteUnit, 16>{
        ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF},
        ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF},
        ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF},
        ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF}, ByteUnit{0xFF}
    });
}

namespace stdx::core {
    template <>
    struct Hash<Uuid> {
        [[nodiscard]]
        usize operator()(const Uuid& uuid) const noexcept {
            usize h = 0;
            for (ByteUnit byte : uuid.bytes()) {
                h ^= Hash<u8>{}(Byte::to_integer<u8>(byte)) + 0x9E3779B9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

SPECIALIZE_HASH(Uuid);
