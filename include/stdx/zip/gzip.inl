#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @class GZIPDeflater
     * @brief Compresses data into the GZIP format (RFC 1952).
     * @extends Deflater
     *
     * Identical to {@code Deflater} but always uses the GZIP wrapper
     * ({@code windowBits = 16 + MAX_WBITS}).
     *
     * @code{.cpp}
     * GZIPDeflater gz;
     * gz.set_input(raw_bytes);
     * ByteBuffer gzip_data = gz.finish();
     * @endcode
     */
    class GZIPDeflater: public Deflater {
    public:
        /**
         * @brief Initializes a GZIP deflater.
         * @param level The compression level.
         * @throws ZipException if zlib initialization fails.
         */
        THROWS(ZipException)
        explicit GZIPDeflater(CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION):
            Deflater(level, WrapperFormat::GZIP) {}
    };

    /**
     * @class GZIPInflater
     * @brief Decompresses GZIP-format data (RFC 1952).
     * @extends Inflater
     *
     * Identical to {@code Inflater} but always expects the GZIP wrapper
     * ({@code windowBits = 16 + MAX_WBITS}).
     *
     * @code{.cpp}
     * GZIPInflater gz;
     * gz.set_input(gzip_bytes);
     * ByteBuffer plain = gz.inflate_all();
     * @endcode
     */
    class GZIPInflater: public Inflater {
    public:
        /**
         * @brief Initializes a GZIP inflater.
         * @throws ZipException if zlib initialization fails.
         */
        THROWS(ZipException)
        explicit GZIPInflater():
            Inflater(WrapperFormat::GZIP) {}
    };

    /**
     * @brief Compresses {@code data} with DEFLATE and returns the result.
     *
     * @param data The bytes to compress.
     * @param level The compression level.
     * @param nowrap If {@code true}, omit the zlib header/trailer.
     * @return The compressed bytes.
     * @throws ZipException if compression fails.
     */
    [[nodiscard]]
    THROWS(ZipException)
    ByteBuffer compress(Span<const u8> data, CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false) {
        Deflater deflater(level, nowrap);
        deflater.set_input(data);
        return deflater.finish();
    }

    /**
     * @brief Compresses the contents of {@code data} with DEFLATE and returns the result.
     *
     * @param data The buffer to compress.
     * @param level The compression level.
     * @param nowrap If {@code true}, omit the zlib header/trailer.
     * @return The compressed bytes.
     * @throws ZipException if compression fails.
     */
    [[nodiscard]]
    THROWS(ZipException)
    ByteBuffer compress(const ByteBuffer& data, CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false) {
        return compress(Span<const u8>(data.data(), data.size()), level, nowrap);
    }

    /**
     * @brief Decompresses {@code data} and returns the original bytes.
     *
     * @param data The compressed bytes.
     * @param nowrap If {@code true}, expect raw deflate data (no zlib header/trailer).
     * @return The decompressed bytes.
     * @throws DataFormatException if the data is corrupt.
     * @throws ZipException if decompression fails.
     */
    [[nodiscard]]
    THROWS(DataFormatException, ZipException)
    ByteBuffer decompress(Span<const u8> data, bool nowrap = false) {
        Inflater inflater(nowrap);
        inflater.set_input(data);
        return inflater.inflate_all();
    }

    /**
     * @brief Decompresses the contents of {@code data} and returns the original bytes.
     *
     * @param data The buffer to decompress.
     * @param nowrap If {@code true}, expect raw deflate data (no zlib header/trailer).
     * @return The decompressed bytes.
     * @throws DataFormatException if the data is corrupt.
     * @throws ZipException if decompression fails.
     */
    [[nodiscard]]
    THROWS(DataFormatException, ZipException)
    ByteBuffer decompress(const ByteBuffer& data, bool nowrap = false) {
        return decompress(Span<const u8>(data.data(), data.size()), nowrap);
    }
}
#endif
