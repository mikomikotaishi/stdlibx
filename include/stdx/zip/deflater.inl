#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
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
        z_stream _stream = z_stream { .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
        ByteBuffer _input; ///< Buffered input set by set_input().
        usize _input_offset; ///< Bytes of input already consumed.
        bool _opened; ///< Whether the zlib stream is live.
        bool _finished_stream; ///< Whether Z_STREAM_END has been seen.

        THROWS(ZipException)
        void init(CompressionLevel level, i32 window_bits) {
            const i32 rc = deflateInit2(&_stream, static_cast<i32>(level), Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
            if (rc != Z_OK) {
                throw ZipException(build_error_message("Failed to initialize deflater", _stream, rc));
            }
            _opened = true;
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
            _input_offset{0}, _opened{false}, _finished_stream{false} {
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
            _input_offset{0}, _opened{false}, _finished_stream{false} {
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

        Deflater(const Deflater&) = DELETE_METHOD("Deflater is not copyable.");
        Deflater& operator=(const Deflater&) = DELETE_METHOD("Deflater is not copyable.");

        ~Deflater() {
            if (_opened) {
                ::deflateEnd(&_stream);
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
            if (!_opened) {
                throw ZipException("Deflater already ended");
            }
            ::deflateEnd(&_stream);
            _opened = false;
        }

        /**
         * @brief Sets the input data to be compressed on the next {@code deflate()} call.
         * @param data The bytes to compress.
         */
        void set_input(Span<const u8> data) {
            _input.assign(data.begin(), data.end());
            _input_offset = 0;
        }

        /**
         * @brief Returns {@code true} if all current input has been consumed.
         * @return {@code true} when more input is needed before the next {@code deflate()}.
         */
        [[nodiscard]]
        bool needs_input() const noexcept {
            return _input_offset >= _input.size();
        }

        /**
         * @brief Returns {@code true} if the stream has been fully flushed and finalized.
         * @return {@code true} after a {@code FlushMode::FINISH} deflate completes.
         */
        [[nodiscard]]
        bool finished() const noexcept {
            return _finished_stream;
        }

        /**
         * @brief Resets the compressor so it can be reused for a new stream.
         * @throws ZipException if the underlying reset fails.
         */
        THROWS(ZipException)
        void reset() {
            const i32 rc = ::deflateReset(&_stream);
            if (rc != Z_OK) {
                throw ZipException(build_error_message("Failed to reset deflater", _stream, rc));
            }
            _finished_stream = false;
            _input.clear();
            _input_offset = 0;
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
            if (!_opened) {
                throw ZipException("Deflater is not initialized");
            }

            if (_finished_stream) {
                return {};
            }

            Array<u8, CHUNK_SIZE> out_chunk = {};
            ByteBuffer out;

            const bool have_input = _input_offset < _input.size();
            const u32 initial_avail = have_input
                ? static_cast<u32>(_input.size() - _input_offset)
                : 0;

            _stream.next_in = have_input
                ? reinterpret_cast<u8*>(_input.data() + _input_offset)
                : nullptr;
            _stream.avail_in = initial_avail;

            i32 rc = Z_OK;
            do {
                _stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
                _stream.avail_out = static_cast<u32>(out_chunk.size());

                rc = ::deflate(&_stream, static_cast<i32>(flush));
                if (rc == Z_STREAM_ERROR || rc == Z_BUF_ERROR) {
                    throw ZipException(build_error_message("Deflate failed", _stream, rc));
                }

                const usize produced = out_chunk.size() - _stream.avail_out;
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
                    _finished_stream = true;
                    break;
                }
            } while (_stream.avail_out == 0 || _stream.avail_in > 0 || (flush == FlushMode::FINISH && rc != Z_STREAM_END));

            if (have_input) {
                const u32 consumed = initial_avail - _stream.avail_in;
                _input_offset += consumed;
                if (_input_offset >= _input.size()) {
                    _input.clear();
                    _input_offset = 0;
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
            return _stream.total_in;
        }

        /**
         * @brief Returns the total number of compressed bytes produced by the stream.
         * @return Total output bytes.
         */
        [[nodiscard]]
        u64 total_out() const noexcept {
            return _stream.total_out;
        }
    };
}
#endif
