#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @enum FlushMode
     * @brief Controls how much output zlib flushes on each {@code deflate()} call.
     */
    enum class FlushMode: u8 {
        NO_FLUSH = Z_NO_FLUSH, ///< Allow zlib to decide how much to flush.
        PARTIAL_FLUSH = Z_PARTIAL_FLUSH, ///< Flush as much output as possible.
        SYNC_FLUSH = Z_SYNC_FLUSH, ///< Flush to a byte boundary.
        FULL_FLUSH = Z_FULL_FLUSH, ///< Flush and reset the compression state.
        FINISH = Z_FINISH, ///< Flush all remaining output and finalize the stream.
        BLOCK = Z_BLOCK, ///< Stop on next block boundary.
        TREES = Z_TREES, ///< Like BLOCK but also output tree headers.
    };

    /**
     * @enum CompressionLevel
     * @brief Trade-off between compression ratio and speed.
     */
    enum class CompressionLevel: i8 {
        NO_COMPRESSION = Z_NO_COMPRESSION, ///< Store data without compression.
        BEST_SPEED = Z_BEST_SPEED, ///< Fastest compression, largest output.
        BEST_COMPRESSION = Z_BEST_COMPRESSION, ///< Slowest compression, smallest output.
        DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION, ///< Balance of speed and ratio.
    };

    /**
     * @enum CompressionStrategy
     * @brief Hint to the compressor about the nature of the input data.
     */
    enum class CompressionStrategy: u8 {
        FILTERED = Z_FILTERED, ///< Optimized for data produced by a filter.
        HUFFMAN_ONLY = Z_HUFFMAN_ONLY, ///< Force Huffman encoding only (no string matching).
        RLE = Z_RLE, ///< Limit match distances to one (RLE-style).
        FIXED = Z_FIXED, ///< Use fixed Huffman codes.
        DEFAULT_STRATEGY = Z_DEFAULT_STRATEGY, ///< General-purpose strategy.
    };

    /**
     * @enum DataType
     * @brief Zlib's best guess at the nature of the data being compressed.
     */
    enum class DataType: u8 {
        BINARY = Z_BINARY, ///< Binary data.
        TEXT = Z_TEXT, ///< Text data (same as ASCII).
        ASCII = Z_ASCII, ///< ASCII text (same as TEXT).
        UNKNOWN = Z_UNKNOWN, ///< Data type not yet determined.
    };

    /**
     * @enum CompressionMethod
     * @brief Compression method used for a ZIP entry.
     */
    enum class CompressionMethod: u16 {
        STORED = 0, ///< No compression; raw data is stored.
        DEFLATED = 8, ///< Compressed with the DEFLATE algorithm.
    };

    /**
     * @enum WrapperFormat
     * @brief Selects the framing applied around a raw deflate stream.
     *
     * Determines how {@code Deflater} packages (or {@code Inflater} interprets)
     * the compressed bytes. The byte payload is always DEFLATE; only the
     * surrounding header and trailer differ.
     */
    enum class WrapperFormat: u8 {
        ZLIB, ///< RFC 1950 zlib wrapping: 2-byte header, Adler-32 trailer.
        RAW, ///< No wrapping; raw DEFLATE bytes with no header or trailer.
        GZIP, ///< RFC 1952 gzip wrapping: 10-byte header, CRC-32 + ISIZE trailer.
    };
}
#endif
