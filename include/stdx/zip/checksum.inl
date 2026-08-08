#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @interface Checksum
     * @brief Interface for rolling checksum computations.
     *
     * Concrete implementations are {@code CRC32} and {@code Adler32}.
     */
    class Checksum {
    public:
        virtual ~Checksum() = default;

        /**
         * @brief Feeds {@code data} into the running checksum.
         * @param data The bytes to include.
         */
        virtual void update(Span<const u8> data) noexcept = 0;

        /**
         * @brief Returns the current checksum value.
         * @return The checksum as a 64-bit unsigned integer.
         */
        [[nodiscard]]
        virtual u64 value() const noexcept = 0;

        /**
         * @brief Resets the checksum to its initial state.
         */
        virtual void reset() noexcept = 0;
    };

    /**
     * @class CRC32
     * @brief CRC-32 checksum, as defined in ISO 3309.
     * @implements Checksum
     */
    class CRC32: public Checksum {
    private:
        u64 _crc_value; ///< Running CRC-32 state.
    public:
        CRC32() noexcept:
            _crc_value{::crc32(0, nullptr, 0)} {}

        void reset() noexcept override {
            _crc_value = ::crc32(0, nullptr, 0);
        }

        void update(Span<const u8> data) noexcept override {
            if (data.empty()) {
                return;
            }
            _crc_value = ::crc32(_crc_value, data.data(), static_cast<u32>(data.size()));
        }

        [[nodiscard]]
        u64 value() const noexcept override {
            return _crc_value;
        }
    };

    /**
     * @class Adler32
     * @brief Adler-32 checksum, as defined in RFC 1950.
     * @implements Checksum
     */
    class Adler32: public Checksum {
    private:
        u64 _adler_value; ///< Running Adler-32 state.
    public:
        Adler32() noexcept:
            _adler_value{::adler32(0, nullptr, 0)} {}

        void reset() noexcept override {
            _adler_value = ::adler32(0, nullptr, 0);
        }

        void update(Span<const u8> data) noexcept override {
            if (data.empty()) {
                return;
            }
            _adler_value = ::adler32(_adler_value, data.data(), static_cast<u32>(data.size()));
        }

        [[nodiscard]]
        u64 value() const noexcept override {
            return _adler_value;
        }
    };
}
#endif
