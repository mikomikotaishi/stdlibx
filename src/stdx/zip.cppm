/**
 * @file zip.cppm
 * @module stdx:zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
#define _BITS_PTHREADTYPES_COMMON_H	1
#include <zlib.h>
#endif

#include "Macros.hpp"

export module stdx:zip;

import :main;

using stdx::io::ByteBuffer;
using stdx::io::IOException;
using stdx::linq::Query;

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
namespace stdx::zip {
    [[nodiscard]]
    inline String build_error_message(StringView prefix, const z_stream& stream, i32 code) {
        String text(prefix);
        text += stdx::fmt::format(" (zlib code {})", code);
        if (stream.msg) {
            text += stdx::fmt::format(": {}", stream.msg);
        }
        return text;
    }
}
#endif

/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {

/**
 * @class ZipException
 * @brief Base exception for ZIP and compression errors.
 * @extends IOException
 */
class ZipException: public IOException {
public:
    using IOException::IOException;
};

/**
 * @class DataFormatException
 * @brief Thrown when compressed data is corrupt or in an unrecognized format.
 * @extends ZipException
 */
class DataFormatException: public ZipException {
public:
    using ZipException::ZipException;
};

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY

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
    u64 crc_value; ///< Running CRC-32 state.
public:
    CRC32() noexcept:
        crc_value{::crc32(0, nullptr, 0)} {}

    void reset() noexcept override {
        crc_value = ::crc32(0, nullptr, 0);
    }

    void update(Span<const u8> data) noexcept override {
        if (data.empty()) {
            return;
        }
        crc_value = ::crc32(crc_value, data.data(), static_cast<u32>(data.size()));
    }

    [[nodiscard]]
    u64 value() const noexcept override {
        return crc_value;
    }
};

/**
 * @class Adler32
 * @brief Adler-32 checksum, as defined in RFC 1950.
 * @implements Checksum
 */
class Adler32: public Checksum {
private:
    u64 adler_value; ///< Running Adler-32 state.
public:
    Adler32() noexcept:
        adler_value{::adler32(0, nullptr, 0)} {}

    void reset() noexcept override {
        adler_value = ::adler32(0, nullptr, 0);
    }

    void update(Span<const u8> data) noexcept override {
        if (data.empty()) {
            return;
        }
        adler_value = ::adler32(adler_value, data.data(), static_cast<u32>(data.size()));
    }

    [[nodiscard]]
    u64 value() const noexcept override {
        return adler_value;
    }
};

/**
 * @struct ZipEntry
 * @brief Metadata for a single entry within a ZIP archive.
 *
 * An entry represents either a file or a directory. Directory entries
 * conventionally have names ending in {@code '/'}.
 */
struct ZipEntry {
    String name; ///< Entry path within the archive (e.g. {@code "dir/file.txt"}).
    String comment; ///< Optional per-entry comment.
    CompressionMethod method = CompressionMethod::DEFLATED; ///< Compression method.
    i64 size = -1; ///< Uncompressed size in bytes, or {@code -1} if unknown.
    i64 compressed_size = -1; ///< Compressed size in bytes, or {@code -1} if unknown.
    u32 crc = 0; ///< CRC-32 checksum of the uncompressed data.
    i64 time = -1; ///< Last-modification time as a DOS-format timestamp, or {@code -1}.

    /**
     * @param name The entry path within the archive.
     */
    explicit ZipEntry(StringView name):
        name{name} {}

    /**
     * @brief Returns {@code true} if this entry represents a directory.
     * @return {@code true} if the name ends with {@code '/'}.
     */
    [[nodiscard]]
    bool is_directory() const noexcept {
        return !name.empty() && name.back() == '/';
    }
};

/**
 * @class Deflater
 * @brief Compresses data using the DEFLATE algorithm.
 *
 * Call {@code set_input()} then {@code deflate()} in a loop until
 * {@code needs_input()} is {@code true}, then feed more data. To finish
 * the stream call {@code finish()}.
 *
 * @code{.cpp}
 * Deflater d;
 * d.set_input(raw_bytes);
 * ByteBuffer compressed = d.finish();
 * @endcode
 */
class Deflater {
public:
    static inline constexpr usize CHUNK_SIZE = 16 * 1024; ///< Internal output chunk size.
private:
    z_stream stream = z_stream { .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
    ByteBuffer input; ///< Buffered input set by set_input().
    usize input_offset; ///< Bytes of input already consumed.
    bool opened; ///< Whether the zlib stream is live.
    bool finished_stream; ///< Whether Z_STREAM_END has been seen.

    THROWS(ZipException)
    void init(CompressionLevel level, i32 window_bits) {
        const i32 rc = deflateInit2(&stream, static_cast<i32>(level), Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
        if (rc != Z_OK) {
            throw ZipException(build_error_message("Failed to initialize deflater", stream, rc));
        }
        opened = true;
    }
public:
    /**
     * @brief Creates a new Deflater for compressing a DEFLATE stream.
     * @param level The compression level.
     * @param nowrap If {@code true}, omit the zlib header/trailer (raw deflate).
     * @throws ZipException if zlib initialization fails.
     */
    THROWS(ZipException)
    explicit Deflater(CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false):
        input_offset{0}, opened{false}, finished_stream{false} {
        init(level, nowrap ? -MAX_WBITS : MAX_WBITS);
    }

    /**
     * @brief Creates a new Deflater for compressing a DEFLATE stream.
     * @param level The compression level.
     * @param format Which framing to wrap the deflate stream in.
     * @throws ZipException if zlib initialization fails.
     */
    THROWS(ZipException)
    Deflater(CompressionLevel level, WrapperFormat format):
        input_offset{0}, opened{false}, finished_stream{false} {
        i32 window_bits = MAX_WBITS;
        switch (format) {
            case WrapperFormat::ZLIB:
                window_bits = MAX_WBITS;
                break;
            case WrapperFormat::RAW:
                window_bits = -MAX_WBITS;
                break;
            case WrapperFormat::GZIP:
                window_bits = 16 + MAX_WBITS;
                break;
        }
        init(level, window_bits);
    }

    Deflater(const Deflater&) = delete("Deflater is not copyable.");
    Deflater& operator=(const Deflater&) = delete("Deflater is not copyable.");

    ~Deflater() {
        if (opened) {
            ::deflateEnd(&stream);
        }
    }

    /**
     * @brief Releases the native zlib stream eagerly.
     *
     * After calling {@code end()}, no other methods may be used.
     * The destructor calls this automatically if not already called.
     *
     * @throws ZipException if the stream was already ended.
     */
    THROWS(ZipException)
    void end() {
        if (!opened) {
            throw ZipException("Deflater already ended");
        }
        ::deflateEnd(&stream);
        opened = false;
    }

    /**
     * @brief Sets the input data to be compressed on the next {@code deflate()} call.
     * @param data The bytes to compress.
     */
    void set_input(Span<const u8> data) {
        input.assign(data.begin(), data.end());
        input_offset = 0;
    }

    /**
     * @brief Returns {@code true} if all current input has been consumed.
     * @return {@code true} when more input is needed before the next {@code deflate()}.
     */
    [[nodiscard]]
    bool needs_input() const noexcept {
        return input_offset >= input.size();
    }

    /**
     * @brief Returns {@code true} if the stream has been fully flushed and finalized.
     * @return {@code true} after a {@code FlushMode::FINISH} deflate completes.
     */
    [[nodiscard]]
    bool finished() const noexcept {
        return finished_stream;
    }

    /**
     * @brief Resets the compressor so it can be reused for a new stream.
     * @throws ZipException if the underlying reset fails.
     */
    THROWS(ZipException)
    void reset() {
        const i32 rc = ::deflateReset(&stream);
        if (rc != Z_OK) {
            throw ZipException(build_error_message("Failed to reset deflater", stream, rc));
        }
        finished_stream = false;
        input.clear();
        input_offset = 0;
    }

    /**
     * @brief Compresses pending input and returns the produced output.
     *
     * May return an empty buffer if zlib has not yet produced output for
     * the given flush mode.
     *
     * @param flush Controls how much output is flushed.
     * @return The compressed bytes produced in this call.
     * @throws ZipException if the compressor is not open or zlib reports an error.
     */
    [[nodiscard]]
    THROWS(ZipException)
    ByteBuffer deflate(FlushMode flush = FlushMode::NO_FLUSH) {
        if (!opened) {
            throw ZipException("Deflater is not initialized");
        }

        if (finished_stream) {
            return {};
        }

        Array<u8, CHUNK_SIZE> out_chunk{};
        ByteBuffer out;

        const bool have_input = input_offset < input.size();
        const u32 initial_avail = have_input
            ? static_cast<u32>(input.size() - input_offset)
            : 0;

        stream.next_in = have_input
            ? reinterpret_cast<u8*>(input.data() + input_offset)
            : nullptr;
        stream.avail_in = initial_avail;

        i32 rc = Z_OK;
        do {
            stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
            stream.avail_out = static_cast<u32>(out_chunk.size());

            rc = ::deflate(&stream, static_cast<i32>(flush));
            if (rc == Z_STREAM_ERROR || rc == Z_BUF_ERROR) {
                throw ZipException(build_error_message("Deflate failed", stream, rc));
            }

            const usize produced = out_chunk.size() - stream.avail_out;
            if (produced > 0) {
                const usize out_old_size = out.size();
                out.resize(out_old_size + produced);
                stdx::util::copy(
                    out_chunk.begin(),
                    out_chunk.begin() + produced,
                    out.data() + static_cast<ptrdiff>(out_old_size)
                );
            }

            if (rc == Z_STREAM_END) {
                finished_stream = true;
                break;
            }
        } while (stream.avail_out == 0 || stream.avail_in > 0 || (flush == FlushMode::FINISH && rc != Z_STREAM_END));

        if (have_input) {
            const u32 consumed = initial_avail - stream.avail_in;
            input_offset += consumed;
            if (input_offset >= input.size()) {
                input.clear();
                input_offset = 0;
            }
        }

        return out;
    }

    /**
     * @brief Flushes all remaining input and finalizes the stream.
     *
     * Equivalent to {@code deflate(FlushMode::FINISH)}.
     *
     * @return All remaining compressed output.
     * @throws ZipException if compression fails.
     */
    [[nodiscard]]
    THROWS(ZipException)
    ByteBuffer finish() {
        return deflate(FlushMode::FINISH);
    }

    /**
     * @brief Returns the total number of uncompressed bytes fed into the stream.
     * @return Total input bytes.
     */
    [[nodiscard]]
    u64 total_in() const noexcept {
        return stream.total_in;
    }

    /**
     * @brief Returns the total number of compressed bytes produced by the stream.
     * @return Total output bytes.
     */
    [[nodiscard]]
    u64 total_out() const noexcept {
        return stream.total_out;
    }
};

/**
 * @class Inflater
 * @brief Decompresses DEFLATE-compressed data.
 *
 * Call {@code set_input()} then {@code inflate()} in a loop until
 * {@code finished()} is {@code true} or {@code needs_input()} is {@code true}.
 *
 * @code{.cpp}
 * Inflater inf;
 * inf.set_input(compressed_bytes);
 * ByteBuffer plain = inf.inflate_all();
 * @endcode
 */
class Inflater {
public:
    static inline constexpr usize CHUNK_SIZE = 16 * 1024; ///< Internal output chunk size.
private:
    z_stream stream = { .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
    ByteBuffer input; ///< Buffered input set by set_input().
    usize input_offset; ///< Bytes of input already consumed.
    bool opened; ///< Whether the zlib stream is live.
    bool finished_stream; ///< Whether Z_STREAM_END has been seen.

    THROWS(ZipException)
    void init(i32 window_bits) {
        const i32 rc = inflateInit2(&stream, window_bits);
        if (rc != Z_OK) {
            throw ZipException(build_error_message("Failed to initialize inflater", stream, rc));
        }
        opened = true;
    }
public:
    /**
     * @brief Creates a new Inflater for decompressing a DEFLATE stream.
     * @param nowrap If {@code true}, expect raw deflate data (no zlib header/trailer).
     * @throws ZipException if zlib initialization fails.
     */
    THROWS(ZipException)
    explicit Inflater(bool nowrap = false):
        opened{false}, finished_stream{false}, input{}, input_offset{0} {
        init(nowrap ? -MAX_WBITS : MAX_WBITS);
    }

    /**
     * @brief Creates a new Inflater for decompressing a DEFLATE stream.
     * @param format Which framing to expect around the deflate stream.
     * @throws ZipException if zlib initialization fails.
     */
    THROWS(ZipException)
    explicit Inflater(WrapperFormat format):
        opened{false}, finished_stream{false}, input{}, input_offset{0} {
        i32 window_bits = MAX_WBITS;
        switch (format) {
            case WrapperFormat::ZLIB:
                window_bits = MAX_WBITS;
                break;
            case WrapperFormat::RAW:
                window_bits = -MAX_WBITS;
                break;
            case WrapperFormat::GZIP:
                window_bits = 16 + MAX_WBITS;
                break;
        }
        init(window_bits);
    }

    Inflater(const Inflater&) = delete("Inflater is not copyable.");
    Inflater& operator=(const Inflater&) = delete("Inflater is not copyable.");

    ~Inflater() {
        if (opened) {
            ::inflateEnd(&stream);
        }
    }

    /**
     * @brief Releases the native zlib stream eagerly.
     *
     * After calling {@code end()}, no other methods may be used.
     * The destructor calls this automatically if not already called.
     *
     * @throws ZipException if the stream was already ended.
     */
    THROWS(ZipException)
    void end() {
        if (!opened) {
            throw ZipException("Inflater already ended");
        }
        ::inflateEnd(&stream);
        opened = false;
    }

    /**
     * @brief Sets the input data to be decompressed on the next {@code inflate()} call.
     * @param data The compressed bytes.
     */
    void set_input(Span<const u8> data) {
        input.assign(data.begin(), data.end());
        input_offset = 0;
    }

    /**
     * @brief Returns {@code true} if all current input has been consumed.
     * @return {@code true} when more input is needed before the next {@code inflate()}.
     */
    [[nodiscard]]
    bool needs_input() const noexcept {
        return input_offset >= input.size();
    }

    /**
     * @brief Returns {@code true} if the end of the compressed stream has been reached.
     * @return {@code true} once Z_STREAM_END is received.
     */
    [[nodiscard]]
    bool finished() const noexcept {
        return finished_stream;
    }

    /**
     * @brief Resets the decompressor so it can be reused for a new stream.
     * @throws ZipException if the underlying reset fails.
     */
    THROWS(ZipException)
    void reset() {
        const i32 rc = ::inflateReset(&stream);
        if (rc != Z_OK) {
            throw ZipException(build_error_message("Failed to reset inflater", stream, rc));
        }
        finished_stream = false;
        input.clear();
        input_offset = 0;
    }

    /**
     * @brief Decompresses pending input and returns the produced output.
     *
     * Returns an empty buffer when no output was produced (e.g. more input is needed).
     *
     * @return The decompressed bytes produced in this call.
     * @throws DataFormatException if the compressed data is corrupt.
     * @throws ZipException if the decompressor is not open or zlib reports a stream error.
     */
    [[nodiscard]]
    THROWS(DataFormatException, ZipException)
    ByteBuffer inflate() {
        if (!opened) {
            throw ZipException("Inflater is not initialized");
        }

        if (finished_stream) {
            return {};
        }

        Array<u8, CHUNK_SIZE> out_chunk{};
        ByteBuffer out;

        const bool have_input = input_offset < input.size();
        const u32 initial_avail = have_input
            ? static_cast<u32>(input.size() - input_offset)
            : 0;

        stream.next_in = have_input
            ? reinterpret_cast<u8*>(input.data() + input_offset)
            : nullptr;
        stream.avail_in = initial_avail;

        i32 rc = Z_OK;
        do {
            stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
            stream.avail_out = static_cast<u32>(out_chunk.size());

            rc = ::inflate(&stream, Z_NO_FLUSH);
            if (rc == Z_NEED_DICT || rc == Z_DATA_ERROR || rc == Z_MEM_ERROR) {
                throw DataFormatException(build_error_message("Inflate failed", stream, rc));
            }
            if (rc == Z_STREAM_ERROR) {
                throw ZipException(build_error_message("Inflate stream error", stream, rc));
            }

            const usize produced = out_chunk.size() - stream.avail_out;
            if (produced > 0) {
                const usize out_old_size = out.size();
                out.resize(out_old_size + produced);
                std::copy(
                    out_chunk.begin(),
                    out_chunk.begin() + static_cast<ptrdiff>(produced),
                    out.data() + static_cast<ptrdiff>(out_old_size)
                );
            }

            if (rc == Z_STREAM_END) {
                finished_stream = true;
                break;
            }

            if (rc == Z_BUF_ERROR && stream.avail_in == 0) {
                break;
            }
        } while (stream.avail_out == 0 || stream.avail_in > 0);

        if (have_input) {
            const u32 consumed = initial_avail - stream.avail_in;
            input_offset += consumed;
            if (input_offset >= input.size()) {
                input.clear();
                input_offset = 0;
            }
        }

        return out;
    }

    /**
     * @brief Decompresses all remaining input and returns it as a single buffer.
     *
     * Calls {@code inflate()} repeatedly until {@code finished()} or
     * {@code needs_input()} is {@code true}.
     *
     * @return The complete decompressed output.
     * @throws DataFormatException if the compressed data is corrupt.
     * @throws ZipException if a stream error occurs.
     */
    [[nodiscard]]
    THROWS(DataFormatException, ZipException)
    ByteBuffer inflate_all() {
        ByteBuffer out;
        while (true) {
            ByteBuffer chunk = inflate();
            if (!chunk.empty()) {
                const usize out_old_size = out.size();
                out.resize(out_old_size + chunk.size());
                stdx::ranges::copy(chunk, out.data() + static_cast<ptrdiff>(out_old_size));
            }
            if (finished_stream || (needs_input() && chunk.empty())) {
                break;
            }
        }
        return out;
    }

    /**
     * @brief Returns the total number of compressed bytes consumed from the stream.
     * @return Total input bytes.
     */
    [[nodiscard]]
    u64 total_in() const noexcept {
        return stream.total_in;
    }

    /**
     * @brief Returns the total number of decompressed bytes produced by the stream.
     * @return Total output bytes.
     */
    [[nodiscard]]
    u64 total_out() const noexcept {
        return stream.total_out;
    }
};

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

#endif

}
